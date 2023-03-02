#ifndef NETTOOLS_HPP_INCLUDED
#define NETTOLS_HPP_INCLUDED

#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define BUF_SIZE 1024

int Socket(int domain, int type, int protocol);

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

ssize_t Send(int sockfd, const void *buf, size_t len, int flags);

ssize_t Recv(int sockfd, void *buf, size_t len, int flags);

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

#endif