
#include "reactor.h"

using namespace std;

void doNothing() {}

void Reactor::pushTask(function<void ()> func)
{
    tasks.put(func);
}

Reactor::~Reactor()
{
    quit.store(true);
    pushTask(&doNothing);
    t.join(); // Variable t of type thread derived from ActiveObject
}

void Reactor::run()
{
    while(!quit.load()) tasks.get()(); // Get a function and call it
}
