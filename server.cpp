#include <iostream>
#include <fstream>
#include <filesystem>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

namespace fs = std::filesystem;

#define PORT 8080

void saveFile(int clientSocket, const std::string& folderName) {
    char buffer[1024] = {0};
    read(clientSocket, buffer, 1024);
    std::string fileName = folderName + "/" + std::string(buffer);

    std::ofstream file(fileName, std::ios::binary);
    int bytesReceived = 0;
    while ((bytesReceived = read(clientSocket, buffer, 1024)) > 0) {
        file.write(buffer, bytesReceived);
    }
    file.close();
}

int main() {
    int server_fd, clientSocket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Ensure folder creation
    fs::create_directories("upload_folder");
    fs::create_directories("download_folder");

    // Creating socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Accepting connection
    if ((clientSocket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    saveFile(clientSocket, "upload_folder");

    close(clientSocket);
    close(server_fd);
    return 0;
}
