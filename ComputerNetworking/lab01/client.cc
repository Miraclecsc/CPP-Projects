#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <chrono>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <hostname>" << std::endl;
        return 1;
    }
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(2680);

    // Convert IPv4 and IPv6 addresses from text to binary form
    // inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr); // localhost
    inet_pton(AF_INET, "10.0.0.1", &serv_addr.sin_addr); // connect server in mininet

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    std::string folderName = std::string(argv[1]);
    mkdir(folderName.c_str(), 0777);  // 创建文件夹

    std::string filePath = folderName + "/received_file.txt";
    std::ofstream outFile(filePath);

    auto start = std::chrono::high_resolution_clock::now();

    int bytesReceived = 0;
    while((bytesReceived = read(sock, buffer, 1024)) > 0) {
        outFile.write(buffer, bytesReceived);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::string logFilePath = folderName + "_client_log.txt";
    std::ofstream logFile(logFilePath);
    logFile << folderName << ":\tTime taken: " << elapsed.count() << " seconds" << std::endl;
    logFile.close();
    outFile.close();
    close(sock);
    return 0;
}