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

    // Funcție pentru a adăuga o sarcină în coada de lucru
    void addTask(std::function<void()> task);

    // Funcție pentru a opri toate firele de execuție
    void stopAllThreads();

private:
    // Funcția care rulează pe fiecare fir de execuție
    static void* threadFunc(void* arg);

    // Numărul de fire de execuție
    int numThreads;

    // Vector pentru stocarea firelor de execuție
    std::vector<pthread_t> threads;

    // Coada de sarcini
    std::queue<std::function<void()>> taskQueue;

    // Mecanisme de sincronizare
    std::mutex queueMutex;
    std::condition_variable_any queueCondition;

    // Flag pentru a indica oprirea firelor
    bool stop;

    // Funcția care gestionează sarcinile
    void executeTasks();
};

#endif
