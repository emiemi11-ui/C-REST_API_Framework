#include "server.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include "shared_queue.hpp"
#include <sys/wait.h>
#include <cstring>
#include "http_response.hpp"

#define SHM_KEY 1234

Server::Server(int port, size_t threadCount) : port(port), pool(threadCount) {}

void Server::start() 
{
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { //server de tip tcp
        perror("socket failed");
        return;
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET; //pt IPv4
    address.sin_addr.s_addr = INADDR_ANY; //ca sa asculte pe toate adresele
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        return;
    }

    if (listen(server_fd, 10) < 0) { //10 nr maxim de conexiuni in asteptare
        perror("listen failed");
        close(server_fd);
        return;
    }

    utils::log("Server (Master) listening on port " + std::to_string(port));

    SharedQueue queue(SHM_KEY, true);

    for (int i = 0; i < 2; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            execl("./worker", "./worker", NULL);
            exit(0);
        }
    }

    while (true) 
    {
        int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) 
            continue;

        pool.enqueue([client_socket]() { //pun cererea in coada in ThreadPool si execut codul in thread
        //citesc cererea
        char buffer[1024] = {0};
        read(client_socket, buffer, sizeof(buffer));

        std::string request(buffer);
        utils::log("[Master] Received new HTTP request");

        SharedQueue queue(SHM_KEY, false);

        Task task;
        strncpy(task.request, request.c_str(), MAX_MSG_SIZE);
        queue.push_request(task); //pun cererea in coada de asteptare

        std::string response_body = queue.pop_response(); //iau raspunsul

        response_body += "\n";

        std::string http = HttpResponse::json(response_body);
        send(client_socket, http.c_str(), http.size(), 0);
        close(client_socket);
    });
    }

    while (wait(nullptr) > 0);
}


