#include "shared_queue.hpp"
#include <cstring>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "semaphore.hpp"

#define SHM_NAME "/pso_shared_queue"

SharedQueue::SharedQueue(key_t key, bool create)
    : isCreator(create)
{
    int flags = O_RDWR;
    if (create)
        flags |= O_CREAT;

    shmid = shm_open(SHM_NAME, flags, 0666);
    if (shmid < 0)
        throw std::runtime_error("shm_open failed");

    if (create) {
        if (ftruncate(shmid, sizeof(SharedData)) < 0)
            throw std::runtime_error("ftruncate failed");
    }

    data = (SharedData*) mmap(nullptr, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);

    if (data == MAP_FAILED)
        throw std::runtime_error("mmap failed");

    req_empty = new Semaphore("/req_empty", MAX_QUEUE_SIZE, create);
    req_full = new Semaphore("/req_full", 0, create);

    resp_empty = new Semaphore("/resp_empty", MAX_QUEUE_SIZE, create);
    resp_full = new Semaphore("/resp_full", 0, create);

    semBin = new Semaphore("/semBin", 1, create);

    if (create) {
        std::memset(data, 0, sizeof(SharedData));
    }
}

SharedQueue::~SharedQueue()
{
    munmap(data, sizeof(SharedData));
    close(shmid);

    delete req_empty;
    delete req_full;
    delete resp_empty;
    delete resp_full;
    delete semBin;

    if (isCreator) {
        shm_unlink(SHM_NAME);
    }
}

void SharedQueue::push_request(const Task& task)
{
    //pune o cerere in coada, doar daca nu este plina
    req_empty->wait();
    semBin->wait();

    std::memcpy( data->req_messages[data->req_rear], task.request, MAX_MSG_SIZE);

    data->req_rear = (data->req_rear + 1) % MAX_QUEUE_SIZE;
    data->req_count++;

    semBin->post();
    req_full->post();
}

Task SharedQueue::pop_request()
{
    //iau o cerere din coada (doar daca nu e goala)
    Task task;

    req_full->wait();
    semBin->wait();

    std::memcpy(task.request, data->req_messages[data->req_front], MAX_MSG_SIZE);

    data->req_front = (data->req_front + 1) % MAX_QUEUE_SIZE;
    data->req_count--;

    semBin->post();
    req_empty->post();

    return task;
}

void SharedQueue::push_response(const std::string& response)
{
    //pune un raspuns in coada, doar daca nu este plina
    resp_empty->wait();
    semBin->wait();

    std::strncpy(data->resp_messages[data->resp_rear], response.c_str(), MAX_MSG_SIZE - 1);
    data->resp_messages[data->resp_rear][MAX_MSG_SIZE - 1] = '\0';

    data->resp_rear = (data->resp_rear + 1) % MAX_QUEUE_SIZE;
    data->resp_count++;

    semBin->post();
    resp_full->post();
}

std::string SharedQueue::pop_response()
{
    //iau un raspuns din coada (doar daca nu e goala)
    resp_full->wait();
    semBin->wait();

    std::string response(data->resp_messages[data->resp_front]);

    data->resp_front = (data->resp_front + 1) % MAX_QUEUE_SIZE;
    data->resp_count--;

    semBin->post();
    resp_empty->post();

    return response;
}

