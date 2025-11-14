#pragma once
#include "service.hpp"
#include <string>

class Controller {
private:
    Service service;
public:
    std::string handleRequest(const std::string& request);
};
