#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>

class Database {
private:
    sqlite3* db;
public:
    Database(const std::string& dbFile);
    ~Database();
    std::vector<std::string> getAllUsers();
    void addUser(const std::string& name);
};
