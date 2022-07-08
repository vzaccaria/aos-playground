#include <asm/io.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/serial_core.h>
#include <linux/spinlock.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/wait.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vittorio Zaccaria & Federico Terraneo");
MODULE_DESCRIPTION("Serial driver built with the Linux Device Framework.");

// Stolen from ttyS1, must edit kernel config to free up port from default
// driver
#define PORT_BASE 0x2F8
#define PORT_SIZE 8
#define PORT_IRQ 3

#define CLK_FREQ 16000000
#define FIFO_SIZE 32

#define UART_MAJOR 248
#define UART_MINOR 1

// 16550 registers, see datasheet
#define RBR 0
#define THR 0
#define IER 1
#define IIR 2
#define FCR 2
#define LSR 5

static unsigned char uart_getc(struct uart_port *port) {
  return (inb(port->iobase + RBR));
}

static irqreturn_t serial_irq(int irq, void *arg) {
  struct uart_port *port = (struct uart_port *)arg;
  struct tty_port *ttyp = &port->state->port;
  // Note, here we should manage errors..
  unsigned char data = uart_getc(port);
  /* printk("Received character '%c'\n", data); */
  tty_insert_flip_char(ttyp, data, TTY_NORMAL);
  tty_flip_buffer_push(&port->state->port);
  return IRQ_HANDLED;
}

static void uart_putc(struct uart_port *port, unsigned char c) {
  // poll until ready to xmit
  while ((inb(port->iobase + LSR) & (1 << 5)) == 0)
    ;
  outb(c, (port->iobase + THR));
}

static void uart_start_tx(struct uart_port *port) {
  struct circ_buf *xmit = &port->state->xmit;
  while (!uart_circ_empty(xmit)) {
    uart_putc(port, xmit->buf[xmit->tail]);
    xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
    port->icount.tx++;
  }
}

static int uart_request_port(struct uart_port *port) {
  printk("Requesting region %lx (size %d) \n", port->iobase, PORT_SIZE);
  if (!request_region(port->iobase, PORT_SIZE, "serialaos")) {
    return -EBUSY;
  }
  return 0;
}

static int uart_startup(struct uart_port *port) {
  printk("Requesting irq %d\n", port->irq);
  printk("Configuring port disabling hardware FIFO, Enabling interrupts when "
         "receiving data\n");
  uart_request_port(port);
  outb(0x0, port->iobase + FCR); // Disable hardware FIFO
  outb(0x5, port->iobase + IER); // Enable RLS, RDA
  return request_irq(port->irq, serial_irq, IRQF_SHARED, "serialaos",
                     (void *)port);
}

static void uart_release_port(struct uart_port *port) {
  printk("Releasing region %lx \n", port->iobase);
  release_region(port->iobase, PORT_SIZE);
}

static void uart_shutdown(struct uart_port *port) { uart_release_port(port); }

static void uart_config_port(struct uart_port *port, int flags) {}

static const char *uart_type(struct uart_port *port) { return "AOS_UART"; }

static void uart_stop_tx(struct uart_port *port) {}

static void uart_stop_rx(struct uart_port *port) {}

static unsigned int uart_tx_empty(struct uart_port *port) {
  /* his function tests whether the transmitter fifo and shifter
  for the port described by 'port' is empty.  If it is empty,
  this function should return TIOCSER_TEMT, otherwise return 0.
  If the port does not support this operation, then it should
  return TIOCSER_TEMT. */
  return TIOCSER_TEMT;
}

static void uart_set_mctrl(struct uart_port *port, unsigned int mctrl) {}

static void uart_set_termios(struct uart_port *port, struct ktermios *new,
                             struct ktermios *old) {}

static struct uart_driver thedriver = {.owner = THIS_MODULE,
                                       .driver_name = "devm_serialaos",
                                       .dev_name = "ttyAOS",
                                       .nr = 1,
                                       .major = UART_MAJOR,
                                       .minor = UART_MINOR};

static struct uart_ops theport_ops = {.tx_empty = uart_tx_empty,
                                      .set_mctrl = uart_set_mctrl,
                                      .get_mctrl = NULL + 16,
                                      .stop_tx = uart_stop_tx,
                                      .start_tx = uart_start_tx,
                                      .stop_rx = uart_stop_rx,
                                      .enable_ms = NULL + 28,
                                      .break_ctl = NULL + 32,
                                      .startup = uart_startup,
                                      .shutdown = uart_shutdown,
                                      .set_termios = uart_set_termios,
                                      .type = uart_type,
                                      .release_port = uart_release_port,
                                      .request_port = uart_request_port,
                                      .config_port = uart_config_port,
                                      .verify_port = NULL + 40};

static struct uart_port theport = {
    .iobase = (unsigned int)PORT_BASE,
    .iotype = UPIO_PORT,
    .irq = PORT_IRQ,
    .ops = &theport_ops,
    /* https://stackoverflow.com/questions/31307651/tiny-serial-driver-example-from-ldd*/
    .type = PORT_16550A,
    .fifosize = 1,
    .line = 0};

struct platform_device *uart_plat_device;

static int __init uart_probe(struct platform_device *dev) {
  uart_add_one_port(&thedriver, &theport);
  platform_set_drvdata(dev, &theport);
  return 0;
}

/* Called when the platform driver is unregistered */
static int uart_remove(struct platform_device *dev) {
  platform_set_drvdata(dev, NULL);
  /* Remove the USB_UART port from the serial core */
  uart_remove_one_port(&thedriver, &theport);
  return 0;
}

static struct platform_driver thepdriver = {
    .probe = uart_probe, /* Probe method */
    .remove = __exit_p(uart_remove),
    /* .suspend = uart_suspend, */
    /* .resume = uart_resume, */
    .driver =
        {
            .name = "devm_serialaos", /* Driver name */
        },
};

static int __init serialaos_init(void) {
  uart_register_driver(&thedriver);
  platform_driver_register(&thepdriver);
  uart_plat_device =
      platform_device_register_simple(thepdriver.driver.name, 0, NULL, 0);
  return 0;
}

static void __exit serialaos_cleanup(void) {
  platform_driver_unregister(&thepdriver);
  platform_device_unregister(uart_plat_device);
  uart_unregister_driver(&thedriver);
}

module_init(serialaos_init);
module_exit(serialaos_cleanup);
