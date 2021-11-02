
#ifndef REACTOR_H_
#define REACTOR_H_

#include <functional>
#include "synchronized_queue.h"
#include "active_object.h"

class Reactor: public ActiveObject
{
public:
    void pushTask(std::function<void ()> func);

    virtual ~Reactor();

private:
    virtual void run();

    SynchronizedQueue<std::function<void ()>> tasks;
};

#endif // REACTOR_H_
