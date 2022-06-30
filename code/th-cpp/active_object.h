
#ifndef ACTIVE_OBJ_H_
#define ACTIVE_OBJ_H_

#include <atomic>
#include <thread>

class ActiveObject
{
public:
    ActiveObject();
    
    virtual ~ActiveObject();
    
private:
    virtual void run();
    
    ActiveObject(const ActiveObject &)=delete;
    ActiveObject& operator=(const ActiveObject &)=delete;
protected:
    std::thread t;
    std::atomic<bool> quit;
};

#endif // ACTIVE_OBJ_H_
