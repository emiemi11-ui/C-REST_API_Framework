#include "utils.hpp"
#include "semaphore.hpp"
#include <iostream>
#include <string>
#include <ctime>

static Semaphore logSem("logSem", 1, true); //controleaza accesul la consola

std::string utils::timestamp() {
    time_t now = time(0);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
}

void utils::log(const std::string &msg) {
    logSem.wait();
    std::cout << "[" << timestamp() << "] " << msg << std::endl;
    logSem.post();
}
