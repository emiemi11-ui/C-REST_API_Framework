#pragma once
#include <pthread.h>
#include <queue>
#include <functional>

class ThreadPool {
private:
    pthread_t* workers;
    size_t thread_count;

    std::queue<std::function<void()>> tasks; //retin cererile

    pthread_mutex_t queue_mutex; //pt sincronizarea accesului la coada de cereri
    pthread_cond_t condition;

    int stop;

    static void* worker_thread(void* arg);

public:
    explicit ThreadPool(size_t threads = 4);
    ~ThreadPool();

    void enqueue(std::function<void()> task); //folosita pt a adauga o sarcina in coada
};
