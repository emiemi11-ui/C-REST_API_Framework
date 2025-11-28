#pragma once
#include "database.hpp"
#include <string>
#include <vector>

class Repository {
private:
    Database db;
public:
    Repository();
    std::vector<std::string> getUsers();
    void addUser(const std::string& name);
};
