#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")  // Winsock Library

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Step 1: Initialize Winsock
    std::cout << "Initializing Winsock..." << std::endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Winsock initialization failed. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Step 2: Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Could not create socket. Error Code: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "Socket created successfully." << std::endl;

    // Step 3: Prepare the sockaddr_in structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Step 4: Bind the socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error Code: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Bind completed successfully." << std::endl;

    // Step 5: Listen for incoming connections
    listen(serverSocket, 3);
    std::cout << "Waiting for incoming connections..." << std::endl;

    // Step 6: Accept an incoming connection
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed. Error Code: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Connection accepted." << std::endl;

    // Step 7: Communicate with the client
    // Add this inside the communication loop in server.cpp (Step 7 in the server code):

while (true) {
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived <= 0) {
        std::cout << "Client disconnected." << std::endl;
        break;
    }
    buffer[bytesReceived] = '\0';  // Null-terminate the received data

    // Check if the client sent "quit"
    if (std::string(buffer) == "quit") {
        std::cout << "Client has requested to disconnect." << std::endl;
        break;
    }

    std::cout << "Client: " << buffer << std::endl;

    // Server sends a response
    std::string response;
    std::cout << "Server: ";
    std::getline(std::cin, response);

    // If the server wants to quit
    if (response == "quit") {
        std::cout << "Disconnecting from client." << std::endl;
        send(clientSocket, response.c_str(), response.size(), 0);
        break;
    }

    send(clientSocket, response.c_str(), response.size(), 0);
}


    // Cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    std::cout << "Server shutdown." << std::endl;
    return 0;
}
