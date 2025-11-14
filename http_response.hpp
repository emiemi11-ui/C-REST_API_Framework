#pragma once
#include <string>
#include <sstream>
#include <unordered_map>

static std::string status_text(int status);
std::string make_http_response(int status, const std::string& body, const std::string& content_type);