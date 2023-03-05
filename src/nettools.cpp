#include "nettools.hpp"
#include "srvcmd.hpp"

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
    bool is_auth = false;
    int conn_status = 0;
    std::string login;
    std::string password;
    //int cmd_status;
    while (true)
    {
        conn_status = Recv(connect_sock, buf, size_buf, 0);
        
        if (!is_auth)
        {
            if (GetCommandStatus != STATUS_LOG)
            {
                strcpy(buf, "Enter login. Command: login <username>\n");
                Send(connect_sock, buf, size_buf, 0);
                continue;
            }
            else 
            {
                login = GetLogin(buf);
                strcpy(buf, "Now enter password. Command: password <password>\n");
                Send(connect_sock, buf, size_buf, 0);
                Recv(connect_sock, buf, size_buf, 0);

                if (GetCommandStatus(buf) != STATUS_PASSWD)
                {
                    strcpy(buf, "Auth failed. Try again from login.\n");
                    Send(connect_sock, buf, size_buf, 0);
                    continue;
                }
                else
                {
                    password = GetPassword(buf);
                    char* ptr_login = login;
                    char* ptr_password = password;
                    if (IsFoundAuthDB(ptr_login, ptr_password))
                    {
                        is_auth = true;
                        strcpy(buf, "You authed successfully\n");
                        Send(connect_sock, buf, size_buf, 0);
                    }
                    else
                    {
                        strcpy(buf, "Account not found!\n");
                        Send(connect_sock, buf, size_buf, 0);
                        continue;
                    }
                }

            }
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
            //session_online = false;
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