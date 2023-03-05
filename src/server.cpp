#include "headers/nettools.hpp"
#include <stdlib.h>
#include <thread>
#include <pqxx/pqxx>

#define PSQL_PASS "aboba" // Enter your postgres password here to connect to db (NOT READY)
#define CONN_COUNT 20

int main(int argc, char** argv)
{
    if (argc != 2 || atoi(argv[1]) == 0) 
    {
        std::cout << "Usage: ./server [port]" << std::endl;
        exit(EXIT_FAILURE);
    }

    
    int cli_sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    
    //cli_sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in srv_addr;

    srv_addr.sin_addr.s_addr = INADDR_ANY;
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(atoi(argv[1]));
    
    std::cout << "Socket successfully created. Waiting for incoming connections..." << std::endl;

    Bind(cli_sockfd, reinterpret_cast<struct sockaddr*>(&srv_addr), sizeof(srv_addr));
    

    

    //Connection to db testing
    try
    {
        //int size_str = sizeof(str);
        pqxx::connection postgres("dbname = postgres user = postgres password = aboba hostaddr = 127.0.0.1 port = 5432");
        if (postgres.is_open())
        {
            std::cout << "Database " << postgres.dbname() << " connected successfully." << std::endl;
        }
        else
        {
            std::cout << "Can't open database." << std::endl;
        }
        postgres.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }; 
    
    int connections[CONN_COUNT];
    int sock_counter = 0;
    char buf[BUF_SIZE];
    bool session_online[CONN_COUNT];
    socklen_t size_addr = sizeof(srv_addr);
    int new_connection;
    
    //end testing
    

    while (true)
    {
        for (int i = 0; i < CONN_COUNT; i++)
        {
            if (connections[i] == 0)
            {
                Listen(cli_sockfd, CONN_COUNT);
                new_connection = Accept(cli_sockfd, reinterpret_cast <struct sockaddr*>(&srv_addr), &size_addr);
                std::strcpy(buf, "Session started, waiting for commands.");
                std::cout << "Connection accepted from client with IP " << inet_ntoa(srv_addr.sin_addr) << std::endl;
                Send(new_connection, buf, BUF_SIZE, 0);
                connections[i] = new_connection;
                //sock_counter++;
                std::thread th(ServerConnectHandler, new_connection, connections + i);
                th.detach();
            }
            
            //session_online[sock_counter] = true;

        }
    }
    
    
        
    close(cli_sockfd);   
        
        
        //std::cout << buf << std::endl;
        //break;

    
    
}

