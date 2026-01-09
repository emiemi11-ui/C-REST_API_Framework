#include "controller.hpp"
#include <sstream>

Controller::Controller(const std::string& dbFile)
    : service(dbFile) {
}

std::string Controller::handleRequest(const std::string& request) 
{

    if (request.rfind("GET /users", 0) == 0) {
        return service.handleGetUsers();
    }

    if (request.rfind("POST /users", 0) == 0) {
        //verifica sa aiba forma corecta si extrage numele
        size_t pos = request.find("\r\n\r\n");
        if (pos == std::string::npos)
            return "{\"error\":\"Missing body\"}";

        std::string body = request.substr(pos + 4);

        if (body.rfind("name=", 0) != 0)
            return "{\"error\":\"Expected name=...\"}";

        std::string name = body.substr(5);
        return service.handleAddUser(name);
    }

    if (request.rfind("DELETE /users", 0) == 0) {
        size_t pos = request.find("\r\n\r\n");
        if (pos == std::string::npos)
            return "{\"error\":\"Missing body\"}";

        std::string body = request.substr(pos + 4);
        if (body.rfind("name=", 0) != 0)
            return "{\"error\":\"Expected name=...\"}";

        std::string name = body.substr(5);
        return service.handleDeleteUserByName(name);
    }

    if (request.rfind("PUT /users", 0) == 0) {
        size_t pos = request.find("\r\n\r\n");
        if (pos == std::string::npos)
            return "{\"error\":\"Missing body\"}";

        std::string body = request.substr(pos + 4);

        size_t sep = body.find("&");
        if (sep == std::string::npos)
            return "{\"error\":\"Expected old=...&new=...\"}";

        std::string oldPart = body.substr(0, sep);
        std::string newPart = body.substr(sep + 1);

        if (oldPart.rfind("old=", 0) != 0 ||
            newPart.rfind("new=", 0) != 0)
            return "{\"error\":\"Expected old=...&new=...\"}";

        std::string oldName = oldPart.substr(4);
        std::string newName = newPart.substr(4);

        return service.handleUpdateUserByName(oldName, newName);
    }


    return "{\"error\":\"Unknown request\"}";
}
