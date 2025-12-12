#pragma once
#include "repository.hpp"
#include <string>

class Service {
private:
    Repository repo;

public:
    std::string handleGetUsers();
    std::string handleAddUser(const std::string& name);
};
