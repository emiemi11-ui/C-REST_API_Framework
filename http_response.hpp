#pragma once
#include <string>

class HttpResponse {
public:
    static std::string json(const std::string& body, int status = 200);
};
