
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

void    ask(char *buffer) {
    int len;
    memset(buffer, 0, 1024);

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
