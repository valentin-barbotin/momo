#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "client.h"
#include "server.h"

#include "utils.h"
#include "user.h"


int main(int argc, char **argv) {
    puts("Hello, world!");
    char    **blacklist;
    int i;

    if (argv[1] == NULL) {
        puts("No arguments given.");
        return 0;
    }

    blacklist = calloc(1, sizeof(char*)); // 50 mots de 128 char max
    if (blacklist == NULL) {
        perror("calloc");
        return 1;
    }

    blacklist[0] = calloc(128, sizeof(char));
    if (blacklist[0] == NULL) {
        perror("calloc");
        return 1;
    }

    // demander les mots a censurer
    puts("Enter words to censor one by one (type /quit to end the list):");
    i = 0;
    do
    {
        ask(blacklist[i]);
        if (strcmp(blacklist[i], "/quit") == 0) {
            break;
        }
        i++;

        blacklist = realloc(blacklist, (i + 1) * sizeof(char*));

        blacklist[i] = calloc(128, sizeof(char));
    } while (1);

    for (size_t j = 0; j < i; j++)
    {
        puts(blacklist[j]);
    }

    // je compare le mot (argv[1]) avec la chaine de caractère "server"
    if (strcmp(argv[1], "server") == 0) {
        mainServer(blacklist, i);
    } else if (strcmp(argv[1], "client") == 0) {
        mainClient(blacklist, i);
    } else {
        puts("Exiting...");
    }
}
