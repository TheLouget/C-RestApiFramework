// ThreadManager.h
#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <pthread.h>
#include <queue>
#include <functional>

class ThreadManager {
public:
    ThreadManager(size_t num_threads = 4);
    ~ThreadManager();
    
    // Add a task to the queue
    void addTask(std::function<void()> task);
    
    // Start the thread pool
    void start();
    
    // Stop the thread pool
    void stop();

private:
    // Thread pool status
    bool running;
    
    // Thread pool and task queue
    std::vector<pthread_t> threads;
    std::queue<std::function<void()>> taskQueue;
    
    // Synchronization primitives
    pthread_mutex_t queueMutex;
    pthread_cond_t condition;
    
    // Thread worker function
    static void* workerFunction(void* arg);
    
    // Helper method that contains the main worker loop
    void workerLoop();
};

#endif // THREAD_MANAGER_H