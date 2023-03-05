#include "nettools.hpp"

std::mutex mtx;

bool is_connection_closed(const char* msg)
{
    std::string operation = msg;
    size_t search_status = operation.find(EXIT_COMMAND);
    //std::cout << EXIT_COMMAND << std::endl;
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

void ServerConnectHandler(int connect_sock, int* active_conn_slot)
{
    char buf[BUF_SIZE];
    socklen_t size_buf = sizeof(buf);
    bool session_online = true;
    int conn_status = 0;
    while (true)
    {
        conn_status = recv(connect_sock, buf, size_buf, 0);
        if (conn_status == -1)
        {
            session_online = false;
            close(connect_sock);
            *active_conn_slot = 0;
            break;
        }
        if (is_connection_closed(buf))
        {
            strcpy(buf, "Connection closed.\n");
            Send(connect_sock, buf, size_buf, 0);
            close(connect_sock);
            *active_conn_slot = 0;
            break;
        }
        conn_status = send(connect_sock, buf, size_buf, MSG_NOSIGNAL);
        if (conn_status == -1)
        {
            session_online = false;
            close(connect_sock);
            *active_conn_slot = 0;
            break;
        }
        mtx.lock();
        std::cout << buf << " : " << connect_sock << std::endl;
        mtx.unlock();
        conn_status = 0;
    }
    

}