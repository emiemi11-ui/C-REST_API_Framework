#include "server.hpp"
#include "utils.hpp"

int main() 
{
    //curl -v http://localhost:8080/
    utils::log("Starting REST API Framework...");
    Server server(8080);
    server.start();
    return 0;
}