#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <netinet/in.h> //pentru socketuri
#include <unistd.h>
#include "utils.hpp"

class Server {
private:
    int port;
    int server_fd; //file descriptorul
    std::vector<std::thread> workers; //un vector de threaduri (fiecare client are propriul thread)

public:
    Server(int port);
    void start();
    void handleClient(int client_socket);
};