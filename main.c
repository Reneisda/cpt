#include <stdio.h>
#include "server.h"
#include "client.h"


int main() {
    int input;
    printf("Receive Files \t \t1\nUpload Files \t \t2\n==> select option\n>");
    scanf("%d", &input);
    if (input == 1) {
        startServer();
    }
    else if (input == 2) {
        clientConnect();
    }
    else
        printf("\nno such option\n");

    return 0;
}

