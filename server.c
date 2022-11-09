#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server.h"
#include "utils.h"
#include "user.h"

void handleClient(int socket) {
    char        buffer[1024] = {0};
    ssize_t     len;

    t_user user;

    puts("What's your username?");
    ask(user.name);

    while (true)
    {
        puts("[server] Waiting for a message...");

        len = recv(socket, buffer, 1024, 0);
        if (len == 0) {
            puts("[server] Server disconnected");
            break;
        }
        if (len < 0) {
            perror("recv");
            break;
        }

        printf("[server] Message received: %s\n", buffer);

        puts("[server] Send msg to client");
        len = send(socket, buffer, strlen(buffer), 0);
        if (len < 0) {
            perror("send");
            break;
        }

        memset(buffer, 0, 1024);

        puts("Insert a message to send to the client");
        ask(buffer);
    }
}


void mainServer() {
    puts("Starting server...");

    int                 sock;
    int                 socketClient;
    struct  sockaddr_in addr;
    struct  sockaddr_in client;
    socklen_t           clientSize;

                    //ipv4, tcp, default protocol
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    printf("Socket created: %d\n", sock);


    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8080);
    addr.sin_family = AF_INET;

    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }

    if (listen(sock, 5) < 0) {
        perror("listen");
        exit(1);
    }

    puts("Server started");

    while (true)
    {
        pid_t pid;
        puts("Waiting for connection...");

        clientSize = sizeof(client);
        socketClient = accept(sock, (struct sockaddr*) &client, &clientSize);
        if (socketClient < 0) {
            perror("accept");
            exit(1);
        }

        puts("Client connected");

        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            // je suis le nouveau processus
            puts("New process");
            handleClient(socketClient);
            // on ira jamais ici
        }

        puts("pierre est un bg");
    }    
}