#pragma once
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include "semaphore.hpp"

#define MAX_QUEUE_SIZE 10
#define MAX_MSG_SIZE 1024

struct Task {
    char request[MAX_MSG_SIZE];
};

struct SharedData {
    char req_messages[MAX_QUEUE_SIZE][MAX_MSG_SIZE];
    int req_front;
    int req_rear;
    int req_count;

    char resp_messages[MAX_QUEUE_SIZE][MAX_MSG_SIZE];
    int resp_front;
    int resp_rear;
    int resp_count;
};

class SharedQueue {
private:
    bool isCreator;
    int shmid;
    SharedData* data;

    Semaphore* req_empty;
    Semaphore* req_full;

    Semaphore* resp_empty;
    Semaphore* resp_full;

    Semaphore* semBin;
public:
    SharedQueue(key_t key, bool create = false);
    ~SharedQueue();

    void push_request(const Task& task);
    Task pop_request();

    void push_response(const std::string& response);
    std::string pop_response();
};

