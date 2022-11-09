#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utils.h"
#include "user.h"

void    handle(int socket, char *buffer, char **blacklist, int blacklistSize) {
    ssize_t     len;
    char        peername[128] = {0};
    t_user      user;

    puts("What's your username?");
    ask(user.name);

    ////////
    len = send(socket, user.name, strlen(user.name), 0);
    if (len < 0) {
        perror("send");
        return;
    }
    ////////
    //recuperer le nom du client
    len = recv(socket, peername, 128, 0);
    if (len == 0) {
        puts("[server] Server disconnected");
        return;
    }
    if (len < 0) {
        perror("recv");
        return;
    }
    ////////

    while (true)
    {
        puts("[client] Send msg to server");

        puts("Message: ");
        ask(buffer);

        // on gere ici les commandes
        if (strcmp(buffer, "/quit") == 0) {
            puts("Exiting...");
            shutdown(socket, SHUT_RDWR);
            close(socket);
            break;
        }

        // censurer dans mon envoi les mots de la blacklist
        censure(blacklist, blacklistSize, buffer);

        len = send(socket, buffer, strlen(buffer), 0);
        if (len < 0) {
            perror("send");
            break;
        }
        printf("[client] Message sent: %s nb: %lu\n", buffer, len);
 
        puts("[client] Waiting for a message...");

        memset(buffer, 0, 1024);
        len = recv(socket, buffer, 1024, 0);
        if (len == 0) {
            puts("[client] Server disconnected");
            break;
        }
        if (len < 0) {
            perror("recv");
            break;
        }

        // on censure les mots recu en regardant la blacklist
        censure(blacklist, blacklistSize, buffer);

        printf("%s: %s\n", peername, buffer);
        // printf("[client] Message received: %s\n", buffer);
    }
}

void    mainClient(char **blacklist, int blacklistSize) {
    puts("Connecting...");

    int                 sock;
    struct  sockaddr_in addr;
    char                buffer[1024] = {0};
    int                 len;

                    //ipv4, tcp, default protocol
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    printf("Socket created: %d\n", sock);

    // puts("Host to connect to?");
    // ask(buffer);

    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8080);
    addr.sin_family = AF_INET;

    int res = connect(sock, (struct sockaddr*) &addr, sizeof(addr));
    if (res < 0) {
        perror("connect");
        exit(1);
    }

    puts("Connected!");


    handle(sock, buffer, blacklist, blacklistSize);
}