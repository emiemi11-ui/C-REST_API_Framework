#include "service.hpp"
#include <sstream>

Service::Service(const std::string& dbFile): respo(dbFile) {
}

std::string Service::handleGetUsers() {
    auto users = respo.getUsers();

    if (users.empty()) {
        return "{\"message\":\"Nu exista utilizatori\"}";
    }

    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < users.size(); i++) {
        os << "\"" << users[i] << "\"";
        if (i < users.size() - 1) os << ", ";
    }
    os << "]";
    return os.str();
}

std::string Service::handleAddUser(const std::string& name) {
    respo.addUser(name);
    return "{\"status\":\"User added successfully\"}";
}

std::string Service::handleDeleteUserByName(const std::string& name) {
    int deleted = respo.deleteUserByName(name);

    if (deleted == 0) {
        return "{\"error\":\"User not found\"}";
    }

    return "{\"status\":\"User deleted\",\"name\":\"" + name + "\"}";
}

std::string Service::handleUpdateUserByName(const std::string& oldName,
                                            const std::string& newName) {
    int updated = respo.updateUserByName(oldName, newName);

    if (updated == 0) {
        return "{\"error\":\"User not found\"}";
    }
    
    return "{\"status\":\"User updated\","
           "\"from\":\"" + oldName +
           "\",\"to\":\"" + newName + "\"}";
}



