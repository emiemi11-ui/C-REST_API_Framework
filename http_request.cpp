#include "http_request.hpp"
#include <sstream>

HttpRequest parse_request_line(const std::string& raw) {
    std::istringstream iss(raw);
    HttpRequest r;
    iss >> r.method >> r.path >> r.version;
    if (r.version.empty()) {
        r.version = "HTTP/1.1";
    }
    return r;
}
