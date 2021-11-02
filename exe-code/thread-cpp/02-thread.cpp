
#include <iostream>
#include <thread>

using namespace std;
using namespace std::chrono;

void myFunc(const string& s)
{
    for(;;)
    {
        cout << s <<endl;
        this_thread::sleep_for(milliseconds(500));
    }
}
int main()
{
    thread t(myFunc, "world");
    myFunc("hello");
}
