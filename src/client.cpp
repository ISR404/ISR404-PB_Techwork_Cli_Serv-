#include "nettools.hpp"
#include <stdlib.h>

int main(int argc, char** argv)
{
    if (argc != 3) 
    {
        std::cout << "Usage: ./client [IP] [port]" << std::endl;
        exit(EXIT_FAILURE);
    }

    int sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in srv_addr;
    in_addr_t address = inet_addr(argv[1]);

    if (address == INADDR_NONE)
    {
        std::cout << "Invalid address. Shutting down..." << std::endl;
        exit(EXIT_FAILURE);
    }

    srv_addr.sin_addr.s_addr = address;
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(atoi(argv[2]));
    
    std::cout << "Socket successfully created. Waiting for server connection" << std::endl;

    int connection;
    char buf[BUF_SIZE];
    bool session_online = false;
    std::string input;

    while (true)
    {
        if (!session_online)
        {
            connection = Connect(sockfd, reinterpret_cast <struct sockaddr*>(&srv_addr), sizeof(srv_addr));
            
            Recv(sockfd, buf, BUF_SIZE, 0);
            std::cout << buf << std::endl;
            session_online = true;
        }
        std::cout << std::endl << ">";
        std::cin.getline(buf, BUF_SIZE);
        if (is_connection_closed(buf))
        {
            Send(sockfd, buf, BUF_SIZE, 0);
            Recv(sockfd, buf, BUF_SIZE, 0);
            std::cout << buf << std::endl;
            close(connection);
            session_online = false;
            break;
        }
        Send(sockfd, buf, BUF_SIZE, 0);
        Recv(sockfd, buf, BUF_SIZE, 0);
        std::cout << buf << std::endl;

    }
    close(sockfd);
}