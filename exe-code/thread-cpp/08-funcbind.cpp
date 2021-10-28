
#include <iostream>
#include <functional>

using namespace std;

void printAdd(int a, int b)
{
    cout<<a<<'+'<<b<<'='<<a+b<<endl;
}

int main()
{
    function<void ()> func;
    func = bind(&printAdd, 2, 3);
    func();
}
