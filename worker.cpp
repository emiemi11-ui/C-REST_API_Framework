#include "shared_queue.hpp"
#include "thread_pool.hpp"
#include "controller.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <iostream>

#define SHM_KEY 1234

// 🔹 Funcția care procesează o cerere din coadă (Controller → Service → Repository → Database)
void processRequest(const std::string& msg) {
    Controller controller;                          // creează un Controller
    std::string response = controller.handleRequest(msg); // procesează cererea HTTP
    utils::log("[Thread] Response: " + response);   // afișează rezultatul
}

// 🔹 Punctul de intrare al procesului Worker
int main() {
    SharedQueue queue(SHM_KEY);     // atașare la memoria partajată
    ThreadPool pool(10);            // 10 thread-uri în fiecare Worker

    utils::log("[Worker] Process started (PID: " + std::to_string(getpid()) + ")");

    while (true) {
        // Worker-ul citește o cerere din coada comună
        std::string msg = queue.pop();
        utils::log("[Worker] Received new task from Master");

        // Fiecare cerere este trimisă la un thread din pool pentru procesare
        pool.enqueue([msg]() {
            processRequest(msg);
        });
    }

    return 0;
}
