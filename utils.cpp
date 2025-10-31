#include "utils.hpp"
#include <iostream>
#include <string>
#include <ctime>

std::string utils::timestamp() {
    time_t now = time(0);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
}

void utils::log(const std::string &msg) {
    std::cout << "[" << timestamp() << "] " << msg << std::endl;
}