#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>

#include "my_bot.h"
#include "utils/socket.h"


int main(int argc, char **argv) {
    int port = 42069;

    if (argc > 2) {
        fprintf(stderr, "Usage: %s [PORT]\n", argv[0]);
        fprintf(stderr, "PORT: port to listen on (default: %d)\n", port);
        exit(1);
    } else if (argc == 2) {
        port = atoi(argv[1]);
    }

    int sockfd = socket_init(port);

    char *class_buf, *size_buf;
    char *msg, *cmd;
    unsigned long size, be_size;

    class_buf = malloc(sizeof(char) * 4);
    size_buf = malloc(sizeof(char) * 8);

    while(1) {
        int clientfd = socket_accept(sockfd);

        socket_recv(clientfd, 4, &class_buf);
        socket_recv(clientfd, 8, &size_buf);

        be_size = *(unsigned long*)size_buf;
        size = be64toh(be_size);

        msg = malloc(sizeof(char) * size + 1);
        socket_recv(clientfd, size, &msg);
        msg[size] = '\0';

        if (strcmp("SHIP", class_buf) == 0) {
            cmd = get_command(msg);
        } else if (strcmp("TOWR", class_buf) == 0) {
            cmd = send_messages(msg);
        } else {
            fprintf(stderr, "Unknown class: %s\n", class_buf);
            exit(1);
        }

        size = strlen(cmd);
        be_size = htobe64(size);

        memcpy(size_buf, &be_size, 8);
        socket_send(clientfd, 8, size_buf);
        socket_send(clientfd, size, cmd);

        free(msg);
        free(cmd);
    }

    return 0;
}