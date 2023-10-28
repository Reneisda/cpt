#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "client.h"
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


int status, client_fd;
struct sockaddr_in serv_addr;
char up_buffer[BUFFER_SIZE];

void startUpload(char *ip, char *path) {
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
    perror("\nInvalid address/ Address not supported \n");
    }

    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
    printf("\nConnection Failed \n");
    }
    FILE *in=fopen(path,"r");
    //Get file length
    fseek(in, 0, SEEK_END);
    unsigned long fileLen=ftell(in);
    fseek(in, 0, SEEK_SET);
    printf("Uploading file [size: %lu]\n", fileLen);
    // sending filesize
    unsigned long sz[1];
    sz[0] = fileLen;
    send(client_fd, sz, sizeof(long), 0);

    char c;
    int read = 0;
    int buffIn = 0;
    while(read < fileLen) {
        c=fgetc(in);
        up_buffer[buffIn] = c;
        read++;
        buffIn++;
        if (buffIn == BUFFER_SIZE) {
            send(client_fd, up_buffer, BUFFER_SIZE, 0);
            buffIn = 0;
        }
    }
    send(client_fd, up_buffer, fileLen % BUFFER_SIZE, 0);

    fclose(in);

    printf("Done\n");
    // closing the connected socket
    close(client_fd);
}
