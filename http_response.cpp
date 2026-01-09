#include "http_response.hpp"

std::string HttpResponse::json(const std::string& body, int status) 
{
    std::string statusText = (status == 200 ? "OK" : "Error");

    return
        "HTTP/1.1 " + std::to_string(status) + " " + statusText + "\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n"
        "Connection: close\r\n\r\n" +
        body;
}
