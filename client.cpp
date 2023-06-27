#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);  // Port number
    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Invalid server address." << std::endl;
        return 1;
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to connect to server." << std::endl;
        return 1;
    }

    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));

    if (recv(clientSocket, buffer, sizeof(buffer) - 1, 0) == -1) {
        std::cerr << "Failed to receive message from server." << std::endl;
    } else {
        std::cout << "Server says: " << buffer << std::endl;
    }

    close(clientSocket);

    return 0;
}

