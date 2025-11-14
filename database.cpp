#include "database.hpp"
#include <iostream>

Database::Database(const std::string& dbFile) {
    if (sqlite3_open(dbFile.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
}

Database::~Database() {
    if (db) sqlite3_close(db);
}

std::vector<std::string> Database::getAllUsers() {
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
    return users;
}

void Database::addUser(const std::string& name) {
    std::string sql = "INSERT INTO users(name) VALUES('" + name + "');";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Insert failed: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}
