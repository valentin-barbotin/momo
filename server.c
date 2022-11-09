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

void handleClient(int socket, char **blacklist, int blacklistSize) {
    char        buffer[1024] = {0};
    char        peername[128] = {0};
    ssize_t     len;
    t_user      user;

    puts("What's your username?");
    ask(user.name);

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
    len = send(socket, user.name, strlen(user.name), 0);
    if (len < 0) {
        perror("send");
        return;
    }
    ////////


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

        // on censure les mots recu en regardant la blacklist
        censure(blacklist, blacklistSize, buffer);

        // printf("[server] Message received: %s\n", buffer);
        printf("%s: %s\n", peername, buffer);

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

        puts("[server] Send msg to client");
        len = send(socket, buffer, strlen(buffer), 0);
        if (len < 0) {
            perror("send");
            break;
        }

        printf("[server] Message sent: %s nb: %lu\n", buffer, len);
    }
}


void mainServer(char **blacklist, int blacklistSize) {
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
            handleClient(socketClient, blacklist, blacklistSize);
            // on ira jamais ici
        }

        puts("pierre est un bg");
    }    
}