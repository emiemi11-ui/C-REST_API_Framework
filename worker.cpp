#include "shared_queue.hpp"
#include "thread_pool.hpp"
#include "controller.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <cstring>

#define SHM_KEY 1234

Controller controller("data.db");

//proceseaza cererea primita
void processRequest(const Task& task, SharedQueue& queue) 
{
    std::string request(task.request);
    std::string response = controller.handleRequest(request);
    queue.push_response(response);

    utils::log("[Worker] Response pushed to server");
}

int main() 
{
    SharedQueue queue(SHM_KEY);
    ThreadPool pool(10);

    utils::log("[Worker] Process started (PID: " + std::to_string(getpid()) + ")");

    while (true) {
        Task task = queue.pop_request(); //extrage cererea din coada partajata
        utils::log("[Worker] Received new task from Master");

        pool.enqueue([task, &queue]() { //acum thredul o proceseaza
            processRequest(task, queue);
        });
    }


    return 0;
}
