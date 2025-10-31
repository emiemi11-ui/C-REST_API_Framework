#include "server.hpp"

Server::Server(int port)
{
    this->port=port;
}

void Server::start() 
{
    struct sockaddr_in address; //strcutura care descrie o adresa ip si un port
    int opt = 1;
    int addrlen = sizeof(address);

    //creez un socket tcp
    //AF_INET = IPv4; SOCK_STREAM = TCP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        return;
    }

    //SO_REUSEADDR => ca sa porneasca serverul imediat pe acelasi port
    //SO_REUSEPORT => permite ca mai multe proecse sa asculte pe acelasi port in paralel
    // 1 ca sa activez aceste optiuni
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET; //IPv4
    address.sin_addr.s_addr = INADDR_ANY; //asculta pe toate interfetele retelei
    address.sin_port = htons(port);

    //bind face legatura socketului cu portul
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        return;
    }

    //int listen(int sockfd, int backlog); => backlog=nr maxim de conexiuni care pot astepta in coada
    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        return;
    }

    utils::log("Server listening on port " + std::to_string(port));

    while (true) {
        //creez un nou socket cu datele clientului
        int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }

        utils::log("Client connected.");
        workers.emplace_back(&Server::handleClient, this, client_socket); //creeaza un nou thread => se executa handleClient
        workers.back().detach(); //sa fie multithread => sa nu blocheze serverul
    }
}

void Server::handleClient(int client_socket) 
{
    char buffer[1024] = {0};
    int bytes_read = read(client_socket, buffer, sizeof(buffer));

    if (bytes_read > 0) {
        utils::log("Received:\n" + std::string(buffer));

        std::string response_body = "Hello from REST API!";
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: " + std::to_string(response_body.size()) + "\r\n"
            "Connection: close\r\n"
            "\r\n" +
            response_body;

        send(client_socket, response.c_str(), response.size(), 0);
        utils::log("Response sent to client");
    }

    close(client_socket);
}
