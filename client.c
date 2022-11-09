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

void    handle(int socket, char *buffer) {
    ssize_t     len;

    t_user user;

    puts("What's your username?");
    ask(user.name);

    while (true)
    {
        puts("[client] Send msg to server");
        len = send(socket, buffer, strlen(buffer), 0);
        if (len < 0) {
            perror("send");
            break;
        }

        puts("[client] Waiting for a message...");

        len = recv(socket, buffer, 1024, 0);
        if (len == 0) {
            puts("[client] Server disconnected");
            break;
        }
        if (len < 0) {
            perror("recv");
            break;
        }

        printf("[client] Message received: %s\n", buffer);

        memset(buffer, 0, 1024);

        puts("Insert a message to send to the client");
        ask(buffer);
    }
}

void    mainClient() {
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

    puts("Host to connect to?");
    ask(buffer);

    addr.sin_addr.s_addr = inet_addr(buffer);
    addr.sin_port = htons(8080);
    addr.sin_family = AF_INET;

    int res = connect(sock, (struct sockaddr*) &addr, sizeof(addr));
    if (res < 0) {
        perror("connect");
        exit(1);
    }

    puts("Connected!");


    handle(sock, buffer);
}