#ifndef UTILS_H
 #define UTILS_H

 #define REMOVE_NEWLINE(str) str[strlen(str) - 1] = '\0'

 void   ask(char *username);
 void   censure(char **blacklist, int blacklistSize, char *buffer);
#endif
