#include "http_response.hpp"
#include <sstream>

static std::string status_text(int status) {
    switch (status) {
        case 200: return "OK";
        case 201: return "Created";
        case 204: return "No Content";
        case 400: return "Bad Request";
        case 404: return "Not Found";
        case 500: return "Internal Server Error";
        default:  return "OK";
    }
}

std::string make_http_response(int status,
                               const std::string& body,
                               const std::string& content_type) {
    std::ostringstream oss;
    oss << "HTTP/1.1 " << status << " " << status_text(status) << "\r\n";
    oss << "Content-Type: " << content_type << "\r\n";
    oss << "Content-Length: " << body.size() << "\r\n";
    oss << "Connection: close\r\n\r\n";
    oss << body;
    return oss.str();
}
