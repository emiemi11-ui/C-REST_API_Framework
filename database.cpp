#include "database.hpp"
#include <iostream>

Database::Database(const std::string& dbFile) {
    pthread_mutex_init(&db_mutex, NULL);

    if (sqlite3_open(dbFile.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
}

Database::~Database() {
    if (db) 
        sqlite3_close(db);
    pthread_mutex_destroy(&db_mutex);
}

std::vector<std::string> Database::getAllUsers() {
    pthread_mutex_lock(&db_mutex);

    std::vector<std::string> users;
    const char* sql = "SELECT name FROM users;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            users.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        }
    } else {
        std::cerr << "Query failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);

    pthread_mutex_unlock(&db_mutex);

    return users;
}

void Database::addUser(const std::string& name) {
    pthread_mutex_lock(&db_mutex);

    std::string sql = "INSERT INTO users(name) VALUES('" + name + "');";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Insert failed: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    pthread_mutex_unlock(&db_mutex);
}

int Database::deleteUserByName(const std::string& name) {
    pthread_mutex_lock(&db_mutex);

    int before = sqlite3_total_changes(db);

    std::string sql =
        "DELETE FROM users WHERE name='" + name + "';";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Delete failed: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    int after = sqlite3_total_changes(db);

    pthread_mutex_unlock(&db_mutex);

    return after - before;
}


int Database::updateUserByName(const std::string& oldName,
                                const std::string& newName) {
    pthread_mutex_lock(&db_mutex);

    int before = sqlite3_total_changes(db);

    std::string sql =
        "UPDATE users SET name='" + newName +
        "' WHERE name='" + oldName + "';";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Update failed: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    int after = sqlite3_total_changes(db);

    pthread_mutex_unlock(&db_mutex);

    return after - before;
}




