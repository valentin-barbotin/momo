#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "client.h"
#include "server.h"

#include "utils.h"
#include "user.h"


int main(int argc, char **argv) {
    puts("Hello, world!");

    if (argv[1] == NULL) {
        puts("No arguments given.");
        return 0;
    }

    // je compare le mot (argv[1]) avec la chaine de caractère "server"
    if (strcmp(argv[1], "server") == 0) {
        mainServer();
    } else if (strcmp(argv[1], "client") == 0) {
        mainClient();
    } else {
        puts("Exiting...");
    }
}
