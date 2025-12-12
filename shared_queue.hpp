#pragma once
#include "semaphore.hpp"
#include "shared_memory.hpp"
#include <string>
#include <cstring>

#define MAX_QUEUE_SIZE 10
#define MAX_MSG_SIZE 1024

struct SharedData {
    char messages[MAX_QUEUE_SIZE][MAX_MSG_SIZE];
    int front;
    int rear;
    int count;
};

class SharedQueue {
private:
    SharedMemory<SharedData> shm;
    Semaphore emptySlots, fullSlots, mutex;

public:
    SharedQueue(key_t key, bool create=false)
    : shm(key, create),
      emptySlots("emptySlots", MAX_QUEUE_SIZE, create),
      fullSlots("fullSlots", 0, create),
      mutex("mutex", 1, create)
{
    if (create) {
        SharedData* d = shm.get();
        d->front = d->rear = d->count = 0;
    }
}

    void push(const std::string& msg) {
        emptySlots.wait();
        mutex.wait();

        SharedData* d = shm.get();
        strncpy(d->messages[d->rear], msg.c_str(), MAX_MSG_SIZE);
        d->rear = (d->rear + 1) % MAX_QUEUE_SIZE;
        d->count++;

        mutex.post();
        fullSlots.post();
    }

    std::string pop() {
        fullSlots.wait();
        mutex.wait();

        SharedData* d = shm.get();
        std::string msg(d->messages[d->front]);
        d->front = (d->front + 1) % MAX_QUEUE_SIZE;
        d->count--;

        mutex.post();
        emptySlots.post();

        return msg;
    }
};
