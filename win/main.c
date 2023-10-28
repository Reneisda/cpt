#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "client.h"
#include "string.h"

#define VERSION "1.0.0"


int main(int argc, char *argv[]) {
    char help[200];
    strcpy(help, "CTransferP2P ");
    strcat(help, VERSION);
    strcat(help, "\nUsage ctp [MODE] [IP] [PATH]\nMODE:\t\t UP|DOWN\nOr bare use with guidance: cpt\n"
                 "-h/--help\t show this\n-v/--version \t show cpt-version");
    if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
            printf("%s\n", help);

        if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
            printf("%s\n", VERSION);
    }
    else if (argc == 4) {
        if (strcmp(argv[1], "down") == 0)
            startServer(argv[2], argv[3]);

        else if (strcmp(argv[1], "up") == 0)
            startUpload(argv[2], argv[3]);
    }
    return 0;
}
