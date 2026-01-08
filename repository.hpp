#pragma once
#include "database.hpp"
#include <string>
#include <vector>

class Repository {
private:
    Database db;
    
public:
    explicit Repository(const std::string& dbFile);
    std::vector<std::string> getUsers();
    void addUser(const std::string& name);
    int deleteUserByName(const std::string& name);
    int updateUserByName(const std::string& oldName, const std::string& newName);
};
