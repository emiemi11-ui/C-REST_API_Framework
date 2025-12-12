#include "semaphore.hpp"
#include <fcntl.h>      // O_CREAT, O_EXCL
#include <sys/stat.h>   // permisiuni 0666
#include <iostream>

Semaphore::Semaphore(const std::string& semName, unsigned int initialValue, bool create)
    : name("/" + semName), owner(create)
{
    if (create) {
        // șterge vechiul semafor dacă există deja
        sem_unlink(name.c_str());
        sem = sem_open(name.c_str(), O_CREAT | O_EXCL, 0666, initialValue);
    } else {
        sem = sem_open(name.c_str(), 0);
    }

    if (sem == SEM_FAILED) {
        perror("sem_open");
        throw std::runtime_error("Failed to open semaphore: " + name);
    }
}

Semaphore::~Semaphore() {
    sem_close(sem);
    if (owner)
        sem_unlink(name.c_str());
}

void Semaphore::wait() {
    sem_wait(sem);
}

void Semaphore::post() {
    sem_post(sem);
}
