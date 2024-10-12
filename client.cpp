#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")  // Winsock Library

#define PORT 8080
#define SERVER_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Step 1: Initialize Winsock
    std::cout << "Initializing Winsock..." << std::endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Winsock initialization failed. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Step 2: Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Could not create socket. Error Code: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "Socket created successfully." << std::endl;

    // Step 3: Prepare the sockaddr_in structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serverAddr.sin_port = htons(PORT);

    // Step 4: Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed. Error Code: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Connected to the server." << std::endl;

    // Step 5: Communicate with the server
    // Add this inside the communication loop in client.cpp (Step 5 in the client code):

while (true) {
    std::string message;
    std::cout << "Client: ";
    std::getline(std::cin, message);

    // Send message to the server
    send(clientSocket, message.c_str(), message.size(), 0);

    // Check if the client wants to quit
    if (message == "quit") {
        std::cout << "Disconnecting from server." << std::endl;
        break;
    }

    // Receive response from the server
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived <= 0) {
        std::cout << "Server disconnected." << std::endl;
        break;
    }
    buffer[bytesReceived] = '\0';  // Null-terminate the received data

    // If the server sent "quit"
    if (std::string(buffer) == "quit") {
        std::cout << "Server has requested to disconnect." << std::endl;
        break;
    }

    std::cout << "Server: " << buffer << std::endl;
}


    // Cleanup
    closesocket(clientSocket);
    WSACleanup();

    std::cout << "Client shutdown." << std::endl;
    return 0;
}
