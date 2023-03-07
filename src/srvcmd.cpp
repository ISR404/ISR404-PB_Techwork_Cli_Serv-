#include "srvcmd.hpp"
#include "nettools.hpp"

std::mutex mtx;

int GetCommandStatus(const char* msg)
{
    std::string operation = msg;
    int search_status; 
    
    search_status = operation.find(CMD_LOGIN/*, 0, sizeof(CMD_LOGIN)/sizeof(char)*/);
    if (search_status == 0)
        return STATUS_LOG;
    search_status = operation.find(CMD_PASSWD/*, 0, sizeof(CMD_LOGIN)/sizeof(char)*/);
    if (search_status == 0)
        return STATUS_PASSWD;
    search_status = operation.find(CMD_LOGOUT/*, 0, sizeof(CMD_LOGIN)/sizeof(char)*/);
    if (search_status == 0)
        return STATUS_LOGOUT;
    search_status = operation.find(CMD_CALC/*, 0, sizeof(CMD_LOGIN)/sizeof(char)*/);
    if (search_status == 0)
        return STATUS_CALC;
    return STATUS_INV;
}

std::string GetLogin(std::string msg)
{
    size_t pos = msg.find_first_of(' ');
    size_t pos_enter = msg.find_first_of('\n');
    std::string login = msg.substr(pos + 1, pos_enter);
    return login;
}

std::string GetPassword(std::string msg)
{
    size_t pos = msg.find_first_of(' ');
    size_t pos_enter = msg.find_first_of('\n');
    std::string password = msg.substr(pos + 1, pos_enter);
    return password;
}

bool IsFoundAuthDB(std::string login, std::string password, const std::string& db_conn_str)
{
    size_t log_size = login.size();
    size_t pass_size = password.size();
    //const char* login_cstr = login.c_str();
    //const char* pass_cstr = password.c_str();
    //char str_char_sql[] = ;
    size_t sql_str_size = sizeof("SELECT * FROM Users WHERE (username = \'\' AND passwd = \'\');");
    size_t dynamic_size = sql_str_size + log_size + pass_size;
    std::cout << log_size << " " << pass_size << " " << sql_str_size << std::endl;
    try
    {
        pqxx::connection postgres(db_conn_str);
        if (postgres.is_open())
        {
            //std::cout << "Database " << postgres.dbname() << " connected successfully." << std::endl;
            char* sql_search = (char*)malloc(sizeof(char) * dynamic_size + 16);
            snprintf(sql_search, /*(sql_str_size + log_size + pass_size + 18)*/ dynamic_size + 16, "SELECT username passwd FROM Users WHERE (username = \'%s\' AND passwd = \'%s\');", login.c_str(), password.c_str());
            std::cout << login.c_str() << " " << password.c_str() << std::endl;
            std::cout << sql_search << std::endl;
            //std::string sql_str = std::format("SELECT * FROM Users WHERE (username = \'{}\' AND passwd = \'{}\');", login, password);
            
            pqxx::nontransaction NTO(postgres);
            pqxx::result rslt(NTO.exec(sql_search));
            free(sql_search);
            if (rslt.empty())
            {
                postgres.close();
                return false;
            }
            else
            {
                postgres.close();
                return true;
            }
        }
        else
        {
            std::cout << "Can't open database." << std::endl;
        }
        
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }; 
    return false;
}


void ServerConnectHandler(const int connect_sock, int* active_conn_slot, const std::string& db_conn_str)
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
            if (GetCommandStatus(buf) != STATUS_LOG)
            {
                std::cout << GetCommandStatus(buf) << std::endl;
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
                    //char* ptr_login = login;
                    //char* ptr_password = password;
                    if (IsFoundAuthDB(login, password, db_conn_str))
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

void FillLogDataDB(std::string &conn_string)
{
    std::string database_name, login, password;
    std::cout << "Before starting, program must take login data to connect to your postgres database. \nEnter database to connect: ";
    std::cin >> database_name;
    std::cout << "Now enter your login: ";
    std::cin >> login;
    std::cout << "And password: ";
    std::cin >> password;
    size_t dynamic_size = sizeof("dbname =  user =  password =  hostaddr = 127.0.0.1 port = 5432") + database_name.size() + login.size() + password.size();
    char* char_conn_str = (char*)malloc(sizeof(char) * dynamic_size);
    snprintf(char_conn_str, dynamic_size, "dbname = %s user = %s password = %s hostaddr = 127.0.0.1 port = 5432", 
    database_name.c_str(), login.c_str(), password.c_str());
    try
    {
        pqxx::connection postgres(char_conn_str);
        conn_string = char_conn_str;
        free(char_conn_str);
        if (postgres.is_open())
        {
            std::cout << std::endl << "Database " << postgres.dbname() << " connected successfully. Ready to process clients." << std::endl;
        }
        else
        {
            std::cout << "Can't open database. Try again. Shutting down..." << std::endl;
            exit(EXIT_FAILURE);
        }
        
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }; 
}

