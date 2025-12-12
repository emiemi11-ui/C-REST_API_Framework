#pragma once
#include <semaphore.h>
#include <string>
#include <stdexcept>

class Semaphore {
private:
    sem_t* sem;
    std::string name;
    bool owner;

public:
    Semaphore(const std::string& semName, unsigned int initialValue = 1, bool create = false);
    ~Semaphore();

    void wait();
    void post();
};
