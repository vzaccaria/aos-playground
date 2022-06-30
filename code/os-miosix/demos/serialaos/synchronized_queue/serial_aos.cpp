/***************************************************************************
 *   Copyright (C) 2021 by Terraneo Federico                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   As a special exception, if other files instantiate templates or use   *
 *   macros or inline functions from this file, or you compile this file   *
 *   and link it with other works to produce a work based on this file,    *
 *   this file does not by itself cause the resulting work to be covered   *
 *   by the GNU General Public License. However the source code for this   *
 *   file must still be made available in accordance with the GNU General  *
 *   Public License. This exception does not invalidate any other reasons  *
 *   why a work based on this file might be covered by the GNU General     *
 *   Public License.                                                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/ 

#include <mutex>
#include "serial_aos.h"
#include "kernel/queue.h"
#include "interfaces/gpio.h"
#include "kernel/scheduler/scheduler.h"

using namespace std;
using namespace miosix;

static mutex txMutex;          ///< Mutex locked during transmission
static mutex rxMutex;          ///< Mutex locked during reception
static Queue<char,64> rxQueue; ///< Queue between rx interrupt and readBlock

/**
 * \internal interrupt routine for usart2 actual implementation
 */
void __attribute__((noinline)) usart2irqImpl()
{
   unsigned int status=USART2->SR;
    if(status & USART_SR_RXNE)
    {
        //Reading the character also clears the interrupt flag
        char c=USART2->DR;
        if((status & USART_SR_FE)==0)
        {
            bool hppw;
            if(rxQueue.IRQput(c,hppw)==false) /*fifo overflow*/;
            if(hppw) Scheduler::IRQfindNextThread();
        }
    }
}

/**
 * \internal interrupt routine for usart2. This is called by the hardware
 */
void __attribute__((naked)) USART2_IRQHandler()
{
    saveContext();
    asm volatile("bl _Z13usart2irqImplv");
    restoreContext();
}

//
// class SerialAOS
//

SerialAOS::SerialAOS() : Device(Device::TTY)
{
    //Some of the registers we're accessing are shared among many peripherals
    //and this code fragment is so short that disabling interrupts is recomended
    InterruptDisableLock dLock;

    //Claim GPIOs for our serial port
    using u2tx=Gpio<GPIOA_BASE,2>;
    using u2rx=Gpio<GPIOA_BASE,3>;
    u2tx::mode(Mode::ALTERNATE);
    u2rx::mode(Mode::ALTERNATE);
    u2tx::alternateFunction(7);
    u2rx::alternateFunction(7);
    
    //Enable clock gating for the serial port peripheral
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    
    //Set our port speed. We want 19200 bit/s. We need to divide serial port
    //peripheral input clock (42MHz) by a factor that gives 16 times 19200Hz
    //(the 16 times requirements is just because that's how this peripheral
    //works) 42MHz / (19200*16) = 136.72, approximated to 136 and 12/16.
    USART2->BRR=136<<4 | 12;

    USART2->CR1 = USART_CR1_UE     //Enable port
                | USART_CR1_RXNEIE //Interrupt on data received
                | USART_CR1_TE     //Transmission enbled
                | USART_CR1_RE;    //Reception enabled
    
    //Finally, we need to set up the interrupt controller
    NVIC_SetPriority(USART2_IRQn,15); //Lowest priority for serial
    NVIC_EnableIRQ(USART2_IRQn);
}

ssize_t SerialAOS::writeBlock(const void *buffer, size_t size, off_t where)
{
    //To protect against multiple threads calling this function
    unique_lock<mutex> l(txMutex);

    //Simplest possible implementation, poor performance: serial ports are slow
    //compared to the CPU, so using polling to send data one chracter at a time
    //is wasteful. The piece that is missing here is to set up the DMA to send
    //the entire buffer in hardware and give us a DMA end of transfer interrupt
    //when the job is done. We are omitting DMA for simplicity.
    const char *buf=reinterpret_cast<const char*>(buffer);
    for(size_t i=0;i<size;i++)
    {
        while((USART2->SR & USART_SR_TXE)==0) ;
        USART2->DR=*buf++;
    }
    return size;
}

ssize_t SerialAOS::readBlock(void *buffer, size_t size, off_t where)
{
    if(size<1) return 0;
    //To protect against multiple threads calling this function
    unique_lock<mutex> l(rxMutex);

    //Simplest possible implementation, poor performance: this time we DO block
    //waiting for data instead of polling (the blocking is hidden in the
    //synchronized queue) but we return after having read only one character.
    //We should try to fill as many bytes of the buffer as possible, BUT also
    //return prematurely if no more chracter arrive. The piece that is missing
    //here is using the peripheral idle line detection, omitted for simplicity.
    char *buf=reinterpret_cast<char*>(buffer);
    char c;
    rxQueue.get(c);
    buf[0]=c;
    return 1;
}
