
#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <atomic>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>
#include "synchronized_queue.h"

class ThreadPool
{
public:
    ThreadPool(int nr_threads = 0);

    virtual ~ThreadPool();

    void pushTask(std::function<void ()> func)
    {
        work_queue.put(func);
    }

    int getWorkQueueLength()
    {
        return work_queue.size();
    }

private:
    ThreadPool(const ThreadPool&)=delete;
    ThreadPool& operator=(const ThreadPool&)=delete;

    void worker_thread();

    std::atomic<bool> done;              //! Thread pool status
    unsigned int thread_count;           //! Thread pool size
    SynchronizedQueue<std::function<void()>> work_queue;
    std::vector<std::thread> threads;    //! Worker threads
};

#endif // THREADPOOL_H_
