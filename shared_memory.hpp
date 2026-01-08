#pragma once
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdexcept>
#include <cstring>

template <typename T>
class SharedMemory {
private:
    int shmid;
    T* data;

public:
    SharedMemory(key_t key, bool create = false) {
        int flags = 0666 | (create ? IPC_CREAT : 0);
        shmid = shmget(key, sizeof(T), flags);
        if (shmid < 0) throw std::runtime_error("shmget failed");

        data = (T*)shmat(shmid, nullptr, 0);
        if (data == (void*)-1) throw std::runtime_error("shmat failed");
    }

    T* get() { return data; }

    void detach() { shmdt(data); }

    void destroy() { shmctl(shmid, IPC_RMID, nullptr); }
};
