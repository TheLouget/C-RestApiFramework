#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <pthread.h>
#include <queue>
#include <functional>
#include <vector>
#include <mutex>
#include <condition_variable>

class ThreadManager {
public:
    ThreadManager(int numThreads);
    ~ThreadManager();
    void addTask(std::function<void()> task);
    void stopAllThreads();

private:
    static void* threadFunc(void* arg);
    int numThreads;
    std::vector<pthread_t> threads;
    std::queue<std::function<void()>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable_any queueCondition;
    bool stop;
    void executeTasks();
};

#endif
