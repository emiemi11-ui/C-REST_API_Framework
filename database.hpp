#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>
#include <pthread.h>

class Database {
private:
    sqlite3* db;
    pthread_mutex_t db_mutex; //pt accesul la baza de date
    
public:
    Database(const std::string& dbFile);
    ~Database();
    std::vector<std::string> getAllUsers();
    void addUser(const std::string& name);
    int deleteUserByName(const std::string& name);
    int updateUserByName(const std::string& oldName, const std::string& newName);
};
