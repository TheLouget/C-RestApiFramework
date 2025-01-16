#include "ThreadManager.h"
#include <stdexcept>

ThreadManager::ThreadManager(size_t num_threads) : running(false) {
    if (pthread_mutex_init(&queueMutex, nullptr) != 0) {
        throw std::runtime_error("Failed to initialize mutex");
    }
    
    if (pthread_cond_init(&condition, nullptr) != 0) {
        pthread_mutex_destroy(&queueMutex);
        throw std::runtime_error("Failed to initialize condition variable");
    }
    
    threads.resize(num_threads);
}

ThreadManager::~ThreadManager() {
    stop();
    pthread_mutex_destroy(&queueMutex);
    pthread_cond_destroy(&condition);
}

void ThreadManager::start() {
    running = true;
    
    for (size_t i = 0; i < threads.size(); ++i) {
        if (pthread_create(&threads[i], nullptr, workerFunction, this) != 0) {
            stop();
            throw std::runtime_error("Failed to create thread");
        }
    }
}

void ThreadManager::stop() {
    pthread_mutex_lock(&queueMutex);
    running = false;
    pthread_cond_broadcast(&condition);
    pthread_mutex_unlock(&queueMutex);
    
    for (pthread_t& thread : threads) {
        pthread_join(thread, nullptr);
    }
    
    while (!taskQueue.empty()) {
        taskQueue.pop();
    }
}

void ThreadManager::addTask(std::function<void()> task) {
    pthread_mutex_lock(&queueMutex);
    taskQueue.push(std::move(task));
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&queueMutex);
}

void* ThreadManager::workerFunction(void* arg) {
    ThreadManager* manager = static_cast<ThreadManager*>(arg);
    manager->workerLoop();
    return nullptr;
}

void ThreadManager::workerLoop() {
    while (true) {
        std::function<void()> task;
        
        pthread_mutex_lock(&queueMutex);
        while (taskQueue.empty() && running) {
            pthread_cond_wait(&condition, &queueMutex);
        }
        
        if (!running && taskQueue.empty()) {
            pthread_mutex_unlock(&queueMutex);
            break;
        }
        
        task = std::move(taskQueue.front());
        taskQueue.pop();
        pthread_mutex_unlock(&queueMutex);
        
        task();
    }
}