#include "server.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include "shared_queue.hpp"
#include <sys/wait.h>

#define SHM_KEY 1234

Server::Server(int port, size_t threadCount) : port(port), pool(threadCount) {}

void Server::start() {
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        return;
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        return;
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        close(server_fd);
        return;
    }

    utils::log("Server (Master) listening on port " + std::to_string(port));

    SharedQueue queue(SHM_KEY, true);

    /*signal(SIGINT, [](int) {
        utils::log("Shutting down server...");
        shm_unlink("/shared_queue");  // sau folosind cheia dacă ai variantă SysV
        exit(0);
    });*/

    for (int i = 0; i < 2; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            // copilul devine un Worker
            execl("./worker", "./worker", NULL);
            exit(0);
        }
    }

    while (true) {
    int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (client_socket < 0) continue;

    //  Rulează procesarea în thread pool
    pool.enqueue([this, client_socket]() {
        char buffer[1024] = {0};
        read(client_socket, buffer, sizeof(buffer));

        std::string request(buffer);
        utils::log("[Master] Received new HTTP request");

        SharedQueue queue(SHM_KEY, false); // false = attach to existing shm
        queue.push(request);                // pune în coada comună

        std::string response = "HTTP/1.1 202 Accepted\r\n\r\nRequest queued\n";
        send(client_socket, response.c_str(), response.size(), 0);
        close(client_socket);
    });
}

    while (wait(nullptr) > 0);
}

void Server::handleClient(int client_socket) {
    char buffer[1024] = {0};
    int bytes_read = read(client_socket, buffer, sizeof(buffer));

    if (bytes_read > 0) {
        std::string request(buffer);
        utils::log("Received:\n" + request);

        std::string response_body = "Hello from REST API!";
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: " + std::to_string(response_body.size()) + "\r\n"
            "Connection: close\r\n\r\n" +
            response_body;

        send(client_socket, response.c_str(), response.size(), 0);
        utils::log("Response sent to client");
    }

    close(client_socket);
}
