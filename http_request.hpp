#pragma once
#include <string>
#include <sstream>
#include <unordered_map>

struct HttpRequest {
    std::string method;
    std::string path;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

HttpRequest parse_request_line(const std::string& raw);