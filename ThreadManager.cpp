#include "ThreadManager.h"
#include <iostream>

ThreadManager::ThreadManager(int numThreads) : numThreads(numThreads), stop(false) {
    for (int i = 0; i < numThreads; ++i) {
        pthread_t thread;
        if (pthread_create(&thread, nullptr, ThreadManager::threadFunc, this) != 0) {
            std::cerr << "Failed to create thread " << i << std::endl;
        }
        threads.push_back(thread);
    }
}

ThreadManager::~ThreadManager() {
    stopAllThreads();
}

void ThreadManager::addTask(std::function<void()> task) {
    std::lock_guard<std::mutex> lock(queueMutex);
    taskQueue.push(task);
    queueCondition.notify_one();
}

void ThreadManager::stopAllThreads() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;
    }
    queueCondition.notify_all();

    for (pthread_t thread : threads) {
        pthread_join(thread, nullptr);
    }
}

void* ThreadManager::threadFunc(void* arg) {
    ThreadManager* manager = static_cast<ThreadManager*>(arg);
    manager->executeTasks();
    return nullptr;
}

void ThreadManager::executeTasks() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);

            // Așteptăm sarcini noi în coadă
            queueCondition.wait(lock, [this]() { return !taskQueue.empty() || stop; });

            if (stop && taskQueue.empty()) {
                break;
            }

            task = taskQueue.front();
            taskQueue.pop();
        }

        // Executăm sarcina în afara blocului critic
        task();
    }
}
