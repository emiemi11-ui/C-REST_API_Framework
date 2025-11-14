#include "controller.hpp"
#include <sstream>

std::string Controller::handleRequest(const std::string& request) {
    if (request.find("GET /users") != std::string::npos) {
        return service.handleGetUsers();
    }
    else if (request.find("POST /users") != std::string::npos) {
        // extragem numele simplu din corpul cererii
        size_t pos = request.find("name=");
        if (pos != std::string::npos) {
            std::string name = request.substr(pos + 5);
            return service.handleAddUser(name);
        }
        return "{\"error\":\"Missing name parameter\"}";
    }
    return "{\"error\":\"Unsupported endpoint\"}";
}
