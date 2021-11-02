
#include <iostream>
#include <chrono>
#include "reactor.h"

using namespace std;
using namespace std::chrono;

void printAdd(int a, int b)
{
    this_thread::sleep_for(seconds(5));
    cout<<a<<'+'<<b<<'='<<a+b<<endl;
}

int main()
{
    Reactor reac;
    int a,b;
    while(cin>>a>>b) reac.pushTask(bind(&printAdd,a,b));
}
