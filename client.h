#ifndef CLIENT_H
 #define CLIENT_H

 void    handle(int socket, char *buffer, char **blacklist, int blacklistSize);
 void mainClient(char **blacklist, int blacklistSize);
#endif
