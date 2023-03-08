#include "nettools.hpp"

bool is_connection_closed(const char* msg)
{
    std::string operation = msg;
    size_t search_status = operation.find(EXIT_COMMAND);
    if (search_status != 0)
        return false;
    return true;

}


int Socket(int domain, int type, int protocol)
{
    int res = socket(domain, type, protocol);
    if (res == -1)
    {
        std::cout << "Socket error!" << std::endl;
        exit(EXIT_FAILURE);
    }
    return res;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int res = bind(sockfd, addr, addrlen);
    if (res == -1)
    {
        std::cout << "Bind error!" << std::endl;
        exit(EXIT_FAILURE);
    }
    return res;
}

int Listen(int sockfd, int backlog)
{
    int res = listen(sockfd, backlog);
    if (res == -1)
    {
        std::cout << "Listen error!" << std::endl;
        exit(EXIT_FAILURE);
    }
    return res;
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int res = accept(sockfd, addr, addrlen);
    if (res == -1)
    {
        std::cout << "Can't accept connection!" << std::endl;
        exit(EXIT_FAILURE);
    }
    return res;
}

ssize_t Send(int sockfd, const void *buf, size_t len, int flags)
{
    int res = send(sockfd, buf, len, flags);
    if (res == -1)
    {
        std::cout << "Sending error!" << std::endl;
        exit(EXIT_FAILURE);
    }
    return res;
}

ssize_t Recv(int sockfd, void *buf, size_t len, int flags)
{
    int res = recv(sockfd, buf, len, flags);
    if (res == -1)
    {
        std::cout << "Receiving error!" << std::endl;
        exit(EXIT_FAILURE);
    }
    return res;
}

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int res = connect(sockfd, addr, addrlen);
    if (res == -1)
    {
        std::cout << "Connection error!" << std::endl;
        std::cout << "Error code: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    return res;
}

