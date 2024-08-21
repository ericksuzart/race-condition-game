#include "../include/Client.hpp"

int main() {
    Client client("127.0.0.1", 8081);
    client.run();
    return 0;
}
