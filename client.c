#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "client.h"
#include "settings.h"

//
// Created by rene on 10/25/23.
//


int status, client_fd;
struct sockaddr_in serv_addr;
char up_buffer[BUFFER_SIZE];

void clientConnect() {
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    }

    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
    printf("\nConnection Failed \n");
    }
    FILE *in=fopen("upload/img.png","r");
    //Get file length
    fseek(in, 0, SEEK_END);
    unsigned long fileLen=ftell(in);
    fseek(in, 0, SEEK_SET);

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
    fclose(in);

    printf("Done\n");
    // closing the connected socket
    close(client_fd);
}
