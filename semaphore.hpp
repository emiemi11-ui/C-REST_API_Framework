#pragma once
#include <semaphore.h>

class Semaphore {
private:
    sem_t* sem;
    const char* name;
    int isCreator;

public:
    Semaphore(const char* semName, unsigned int initialValue, int create);
    ~Semaphore();

    void wait();
    void post();
};
