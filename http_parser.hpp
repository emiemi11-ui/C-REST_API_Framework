#pragma once
#include <string>
#include <sstream>
#include <unordered_map>

struct HttpRequest {
    std::string method;
    std::string path;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

class HttpParser {
public:
    static HttpRequest parse(const std::string& raw) {
        HttpRequest req;
        std::istringstream stream(raw);
        std::string line;

        // Prima linie: metoda și ruta
        if (std::getline(stream, line)) {
            std::istringstream first(line);
            first >> req.method >> req.path;
        }

        // Header-ele
        while (std::getline(stream, line) && line != "\r") {
            size_t pos = line.find(": ");
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 2);
                if (!value.empty() && value.back() == '\r')
                    value.pop_back();
                req.headers[key] = value;
            }
        }

        // Body (restul)
        std::string body;
        while (std::getline(stream, line)) {
            body += line + "\n";
        }
        req.body = body;
        return req;
    }
};
