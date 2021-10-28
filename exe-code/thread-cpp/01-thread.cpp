
#include <iostream>
#include <thread>

using namespace std;
using namespace std::chrono;

void myThread()
{
    for(;;)
    {
        cout<<"world "<<endl;
        this_thread::sleep_for(milliseconds(500));
    }
}
int main()
{
    thread t(myThread);
    for(;;)
    {
        cout<<"Hello"<<endl;
        this_thread::sleep_for(milliseconds(500));
    }
}
