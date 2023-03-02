#include "nettools.hpp"
#include <stdlib.h>
//#include <thread>

int main(int argc, char** argv)
{
    if (argc != 2 || atoi(argv[1]) == 0) 
    {
        std::cout << "Usage: ./server [port]" << std::endl;
        exit(EXIT_FAILURE);
    }

    
    int cli_sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in srv_addr;
    //in_addr_t address = inet_addr(argv[1]);

    /* if (address == INADDR_NONE)
    {
        std::cout << "Invalid address. Shutting down..." << std::endl;
        exit(EXIT_FAILURE);
    } */

    srv_addr.sin_addr.s_addr = INADDR_ANY;
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(atoi(argv[1]));
    
    std::cout << "Socket successfully created. Waiting for incoming connections..." << std::endl;

    Bind(cli_sockfd, reinterpret_cast<struct sockaddr*>(&srv_addr), sizeof(srv_addr));

    int connection;
    char buf[BUF_SIZE];
    bool session_online = false;
    std::string input;
    std::string received;
    socklen_t size_addr = sizeof(srv_addr);

    while (true)
    {
        if (!session_online)
        {
            Listen(cli_sockfd, 5);
            connection = Accept(cli_sockfd, reinterpret_cast <struct sockaddr*>(&srv_addr), &size_addr);
            std::strcpy(buf, "Session started, waiting for commands.");
            std::cout << "Connection accepted from client with IP " << inet_ntoa(srv_addr.sin_addr) << std::endl;
            Send(connection, buf, BUF_SIZE, 0);
            session_online = true;
        }
        Recv(connection, buf, BUF_SIZE, 0);
        if (is_connection_closed(buf))
        {
            std::strcpy(buf, "Connection closed.\n");
            Send(connection, buf, BUF_SIZE, 0);
            std::cout << buf;
            close(connection);
            session_online = false;
        }
        else
        {
            std::strcpy(buf, "Message received.");
            Send(connection, buf, BUF_SIZE, 0);
        }
        
        
        //std::cout << buf << std::endl;
        //break;

    }
    close(cli_sockfd);
}

