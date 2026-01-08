#include "thread_pool.hpp"
#include <stdlib.h>

ThreadPool::ThreadPool(size_t threads)
    : thread_count(threads), stop(0)
{
    pthread_mutex_init(&queue_mutex, nullptr);
    pthread_cond_init(&condition, nullptr);

    workers = (pthread_t*)malloc(sizeof(pthread_t) * thread_count);

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_create(&workers[i], nullptr, worker_thread, this);
    }
}

void* ThreadPool::worker_thread(void* arg) 
{
    ThreadPool* pool = (ThreadPool*)arg;

    while (1) {
        std::function<void()> task;

        pthread_mutex_lock(&pool->queue_mutex);
        while (!pool->stop && pool->tasks.empty()) { //thredul asteapta o cerere
            pthread_cond_wait(&pool->condition, &pool->queue_mutex);
        }

        if (pool->stop && pool->tasks.empty()) { //daca s-a semnalizat oprirea prin stop si coada e goala
            pthread_mutex_unlock(&pool->queue_mutex);
            pthread_exit(nullptr);
        }

        task = pool->tasks.front(); //extrage cererea 
        pool->tasks.pop(); //o sterge
        pthread_mutex_unlock(&pool->queue_mutex);

        task(); //rezolv cererea intr-o functie lambda
    }
}

//adaug o cerere in coada
void ThreadPool::enqueue(std::function<void()> task) 
{
    pthread_mutex_lock(&queue_mutex);
    tasks.push(task);
    pthread_mutex_unlock(&queue_mutex);

    pthread_cond_signal(&condition);
}

ThreadPool::~ThreadPool() 
{
    pthread_mutex_lock(&queue_mutex);
    stop = 1;
    pthread_mutex_unlock(&queue_mutex);

    pthread_cond_broadcast(&condition); //toate thredurile sa iasa din executie

    for (size_t i = 0; i < thread_count; i++) {
        pthread_join(workers[i], nullptr);
    }

    free(workers);

    pthread_mutex_destroy(&queue_mutex);
    pthread_cond_destroy(&condition);
}
