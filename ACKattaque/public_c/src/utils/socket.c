#include "socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


int socket_init(int port)
{
    int status;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = htonl(INADDR_LOOPBACK)
    };

    status = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (status == -1) {
        perror("bind");
        exit(1);
    }

    status = listen(sockfd, 1);
    if (status == -1) {
        perror("listen");
        exit(1);
    }

    printf("Listening on port %d\n", port);

    return sockfd;
}

int socket_accept(int sockfd)
{
    int clientfd = accept(sockfd, NULL, NULL);
    if (clientfd == -1) {
        perror("accept");
        exit(1);
    }

    return clientfd;
}

int socket_send(int clientfd, int size, char *msg)
{
    int n_bytes = write(clientfd, msg, size);
    if (n_bytes == -1) {
        perror("write");
        exit(1);
    }

    return n_bytes;
}

int socket_recv(int clientfd, int size, char **buf)
{
    int n_bytes = 0;
    while (n_bytes < size) {
        n_bytes += read(clientfd, *buf + n_bytes, size - n_bytes);
    }

    return n_bytes;
}
