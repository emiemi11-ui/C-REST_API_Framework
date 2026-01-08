#include "repository.hpp"

Repository::Repository(const std::string& dbFile)
    : db(dbFile) {
}

std::vector<std::string> Repository::getUsers() {
    return db.getAllUsers();
}

void Repository::addUser(const std::string& name) {
    db.addUser(name);
}

int Repository::deleteUserByName(const std::string& name) {
    return db.deleteUserByName(name);
}

int Repository::updateUserByName(const std::string& oldName, const std::string& newName) {
    return db.updateUserByName(oldName, newName);
}
