#pragma once
#include <string>
#include <netinet/in.h>
#include "thread_pool.hpp"

class Server {
private:
    int port;
    int server_fd;
    ThreadPool pool;

public:
    Server(int port, size_t threadCount = 8);
    void start();
};
