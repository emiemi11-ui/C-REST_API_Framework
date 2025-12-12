#include "service.hpp"
#include <sstream>

std::string Service::handleGetUsers() {
    auto users = repo.getUsers();
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < users.size(); ++i) {
        os << "\"" << users[i] << "\"";
        if (i < users.size() - 1) os << ", ";
    }
    os << "]";
    return os.str();
}

std::string Service::handleAddUser(const std::string& name) {
    repo.addUser(name);
    return "{\"status\":\"User added successfully\"}";
}
