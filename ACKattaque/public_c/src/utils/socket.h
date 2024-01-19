#ifndef SOCKET_H
#define SOCKET_H

int socket_init(int port);
int socket_accept(int socket);
int socket_send(int socket, int size, char *msg);
int socket_recv(int socket, int size, char **buf);

#endif // SOCKET_H
