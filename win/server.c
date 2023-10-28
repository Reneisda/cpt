#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "settings.h"
#ifdef __linux__
    #include <sys/socket.h>
    #include <netinet/in.h>

#elif _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")

#else
    #error Platform not supported
#endif
//
// Created by rene on 10/25/23.
//


int server_fd, new_socket;
int opt = 1;
ssize_t valread;
struct sockaddr_in address;
socklen_t addrlen = sizeof(address);
char buffer[BUFFER_SIZE] = {};

void startServer(char *ip, char* path) {
    printf("starting server\n");
    if ((server_fd = socket(AF_INET, SOCK_STREAM, PROTOCOL_IP)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET,PO_REN_PORT | SO_REUSEPORT, &opt,sizeof(opt))) {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    printf("hosting on %s ", ip);
    if (inet_pton(AF_INET, ip, &address.sin_addr) <= 0) {
        perror("\nInvalid address/ Address not supported \n");
    }
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr*)&address,sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for client to connect\n");
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address,&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    unsigned long fileLen[1];
    fileLen[0] = 0;
    read(new_socket, fileLen, sizeof(long));
    printf("FileSize: %lu\n", fileLen[0]);
    FILE *ptr;
    ptr = fopen(path,"wb");  // r for read, b for binary
    unsigned long bytesRead = 0;
    while (bytesRead < fileLen[0] - BUFFER_SIZE) {
        read(new_socket, buffer, BUFFER_SIZE);
        bytesRead += BUFFER_SIZE;
        fwrite(buffer,BUFFER_SIZE,1,ptr);
    }
    if (fileLen[0] % BUFFER_SIZE != 0) {
        read(new_socket, buffer, fileLen[0] % BUFFER_SIZE);
        fwrite(buffer, fileLen[0] % BUFFER_SIZE, 1, ptr);
    }
    close(new_socket);
    // closing the listening socket
    close(server_fd);
    printf("Done\n");
}