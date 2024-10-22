#include <iostream>
#include <fstream>
#include <filesystem>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

namespace fs = std::filesystem;

#define PORT 8080

void uploadFile(int socket, const std::string& fileName) {
    char buffer[1024] = {0};
    send(socket, fileName.c_str(), fileName.size(), 0);

    std::ifstream file(fileName, std::ios::binary);
    while (file.read(buffer, sizeof(buffer))) {
        send(socket, buffer, file.gcount(), 0);
    }
    file.close();
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    std::string fileName = "test.txt";  // example file
    uploadFile(sock, fileName);

    close(sock);
    return 0;
}
