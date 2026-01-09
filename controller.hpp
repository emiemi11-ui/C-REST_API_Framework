#pragma once
#include "service.hpp"

class Controller {
private:
    Service service;

public:
    explicit Controller(const std::string& dbFile);
    std::string handleRequest(const std::string& request);
};
