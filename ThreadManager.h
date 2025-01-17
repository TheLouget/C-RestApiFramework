
#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <pthread.h>
#include <queue>
#include <functional>

class ThreadManager {
public:
    ThreadManager(size_t num_threads = 4);
    ~ThreadManager(); 
    void addTask(std::function<void()> task);
    void start();
    void stop();
private:
    bool running;
    std::vector<pthread_t> threads;
    std::queue<std::function<void()>> taskQueue;
    pthread_mutex_t queueMutex;
    pthread_cond_t condition;
    static void* workerFunction(void* arg);
    void workerLoop();
};

#endif // THREAD_MANAGER_H