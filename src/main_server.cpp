#include "../include/Server.hpp"

int main() {
    Server server(8081);
    server.start();
    return 0;
}
