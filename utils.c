
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

void    ask(char *buffer) {
    int len;
    *buffer = 0;
    // memset(buffer, 0, 1024);

    do
    {
        fgets(buffer, 128, stdin);
        REMOVE_NEWLINE(buffer);

        len = strlen(buffer);
        if (!len) {
            puts("Please enter a text.");
        }
    } while (!len);
}

void    censure(char **blacklist, int blacklistSize, char *buffer) {
    for (int i = 0; i < blacklistSize; i++)
    {
        char *pos = strstr(buffer, blacklist[i]);
        if (pos) {
            int len = strlen(blacklist[i]);
            for (int j = 0; j < len; j++)
            {
                pos[j] = '*';
            }
        }
    }
}
