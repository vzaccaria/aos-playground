
#include <iostream>
#include <chrono>
#include "thread_pool.h"

using namespace std;
using namespace std::chrono;

void printAdd(int a, int b)
{
    this_thread::sleep_for(seconds(5));
    cout<<a<<'+'<<b<<'='<<a+b<<endl;
}

int main()
{
    ThreadPool reac;
    int a,b;
    while(cin>>a>>b) reac.pushTask(bind(&printAdd,a,b));
}
