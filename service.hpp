#pragma once
#include "repository.hpp"
#include <string>

class Service {
private:
    Repository respo;
public:

    explicit Service(const std::string& dbFile);
    std::string handleGetUsers();
    std::string handleAddUser(const std::string& name);
    std::string handleDeleteUserByName(const std::string& name);
    std::string handleUpdateUserByName(const std::string& oldName, const std::string& newName);
};
