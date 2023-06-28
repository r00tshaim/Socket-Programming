#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

int main() {
    //create a server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }
    
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);  // Port number
    serverAddress.sin_addr.s_addr = INADDR_ANY;  // Bind to any available interface

    //assign ip and port to socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind socket." << std::endl;
        return 1;
    }

    //tell server socket to listen and keep only 2 connection in queue, others will be rejected
    if (listen(serverSocket, 2) == -1) {
        std::cerr << "Failed to listen for connections." << std::endl;
        return 1;
    }

    std::cout << "Server listening for incoming connections..." << std::endl;

    while (true) {
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);

	/*The accept() function takes as arguments the main(ie server) socket file descriptor, 
	 * a pointer to a sockaddr structure that will be filled with the address of the connecting client, 
	 * and a pointer to an integer that contains the size of the sockaddr structure. 
	 * The function returns a new socket file descriptor that can be used to communicate with the connected client.*/
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket == -1) {
            std::cerr << "Failed to accept client connection." << std::endl;
            continue;
        }

        char clientAddressStr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddress.sin_addr), clientAddressStr, INET_ADDRSTRLEN);
        std::cout << "Client connected: " << clientAddressStr << std::endl;

        const char* message = "Hello, client!";
        if (send(clientSocket, message, std::strlen(message), 0) == -1) {
            std::cerr << "Failed to send message to client." << std::endl;
        }

	char buffer[1024];
    	std::memset(buffer, 0, sizeof(buffer));
	
	while (true) {
        	if (recv(clientSocket, buffer, sizeof(buffer) - 1, 0) == -1) {
            		std::cerr << "Failed to receive message from client." << std::endl;
            		break;
        	}

        	std::cout << "Client says: " << buffer << std::endl;

        	if (std::strcmp(buffer, "bye") == 0) {
			//client requesting disconnect
            		std::cout << "Client initiated disconnect." << std::endl;
			send(clientSocket, buffer, std::strlen(buffer), 0);
            		break;
        	}

        	std::memset(buffer, 0, sizeof(buffer));

        	std::cout << "Server says: ";
        	std::cin.getline(buffer, sizeof(buffer));

        	if (send(clientSocket, buffer, std::strlen(buffer), 0) == -1) {
            		std::cerr << "Failed to send message to client." << std::endl;
            		break;
        	} else {
			//message is sent to client, we can clear the buffer now
			std::memset(buffer, 0, sizeof(buffer));
		}

    	}
	sleep(5);
	close(clientSocket);
    }

    close(serverSocket);

    return 0;
}
