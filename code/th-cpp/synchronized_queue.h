
#ifndef SYNC_QUEUE_H_
#define SYNC_QUEUE_H_

#include <list>
#include <mutex>
#include <condition_variable>

template<typename T>
class SynchronizedQueue
{
public:
    SynchronizedQueue() {}
    
    void put(const T & data);
    
    T get();
    
    size_t size();

private:
    SynchronizedQueue(const SynchronizedQueue &)=delete;
    SynchronizedQueue & operator=(const SynchronizedQueue &)=delete;
    
    std::list<T> queue;
    std::mutex myMutex;
    std::condition_variable myCv;
};

template<typename T>
void SynchronizedQueue<T>::put(const T& data)
{
    std::unique_lock<std::mutex> lck(myMutex);
    queue.push_back(data);
    myCv.notify_one();
}
template<typename T>
T SynchronizedQueue<T>::get()
{
    std::unique_lock<std::mutex> lck(myMutex);
    while(queue.empty()) myCv.wait(lck);
    T result=queue.front();
    queue.pop_front();
    return result;
}

template<typename T>
size_t SynchronizedQueue<T>::size()
{
    std::unique_lock<std::mutex> lck(myMutex);
    return queue.size();
}

#endif // SYNC_QUEUE_H_
