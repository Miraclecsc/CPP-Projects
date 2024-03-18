#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <stdlib.h>
#include <arpa/inet.h>

void handle_client(int client_socket) {
    const char *filename = "/root/lab01/file.txt";
    char buffer[1024] = {0};
    std::ifstream file(filename);
    if (file.is_open()) {
        while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) 
        {
            send(client_socket, buffer, file.gcount(), 0);
        }
        file.close();
    } 
    else {
        std::cout << "File not found" << std::endl;
    }
    close(client_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(2680);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            continue;
        }
        std::thread client_thread(handle_client, new_socket);
        client_thread.detach();
    }
    close(server_fd);
    return 0;
}