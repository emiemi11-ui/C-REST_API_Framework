#include "repository.hpp"

Repository::Repository() : db("data.db") {} 

std::vector<std::string> Repository::getUsers() {
    return db.getAllUsers();
}

void Repository::addUser(const std::string& name) {
    db.addUser(name);
}

//Repository pruimeste comanda de la Service si o da mai departe catre baza de date
