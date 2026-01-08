#include "semaphore.hpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

Semaphore::Semaphore(const char* semName, unsigned int initialValue, int create)
    : name(semName), isCreator(create)
{
    if (create) {
        sem = sem_open(name, O_CREAT, 0666, initialValue);
    } else {
        sem = sem_open(name, 0);
    }

    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }
}

Semaphore::~Semaphore() {
    sem_close(sem);

    if (isCreator) {
        sem_unlink(name);
    }
}

void Semaphore::wait() {
    sem_wait(sem);
}

void Semaphore::post() {
    sem_post(sem);
}
