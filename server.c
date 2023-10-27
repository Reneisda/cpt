#include "server.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include "settings.h"
//
// Created by rene on 10/25/23.
//


int server_fd, new_socket;
int opt = 1;
ssize_t valread;
struct sockaddr_in address;
socklen_t addrlen = sizeof(address);
char buffer[BUFFER_SIZE] = {};

void startServer() {
    printf("starting server\n");
    if ((server_fd = socket(AF_INET, SOCK_STREAM, PROTOCOL_IP)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
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
    FILE *ptr;

    ptr = fopen("down.png","wb");  // r for read, b for binary
    while (read(new_socket, buffer, BUFFER_SIZE) != 0) {
        printf("recv chunk\n");
        fwrite(buffer,BUFFER_SIZE,1,ptr);
    }
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        printf("%x", buffer[i]);

    }
    fwrite(buffer,BUFFER_SIZE,1,ptr);
    close(new_socket);
    // closing the listening socket
    close(server_fd);
}