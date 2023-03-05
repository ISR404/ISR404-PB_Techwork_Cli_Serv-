#include "srvcmd.hpp"
//#include "nettools.hpp"


int GetCommandStatus(const char* msg)
{
    std::string operation = msg;
    int search_status; 
    
    search_status = operation.find(CMD_LOGIN, 0, sizeof(CMD_LOGIN)/sizeof(char));
    if (search_status == 0)
        return STATUS_LOG;
    search_status = operation.find(CMD_PASSWD, 0, sizeof(CMD_LOGIN)/sizeof(char));
    if (search_status == 0)
        return STATUS_PASSWD;
    search_status = operation.find(CMD_LOGOUT, 0, sizeof(CMD_LOGIN)/sizeof(char));
    if (search_status == 0)
        return STATUS_LOGOUT;
    search_status = operation.find(CMD_CALC, 0, sizeof(CMD_LOGIN)/sizeof(char));
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

bool IsFoundAuthDB(char* login, char* password)
{
    size_t log_size = sizeof(login);
    size_t pass_size = sizeof(password);
    size_t sql_str_size = sizeof("SELECT username passwd FROM Users WHERE (username = %s AND passwd = %s);");
    try
    {
        pqxx::connection postgres("dbname = postgres user = postgres password = aboba hostaddr = 127.0.0.1 port = 5432");
        if (postgres.is_open())
        {
            //std::cout << "Database " << postgres.dbname() << " connected successfully." << std::endl;
            char* sql_search;
            snprintf(sql_search, sql_str_size + log_size + pass_size, "SELECT username passwd FROM Users WHERE \
            (username = %s AND passwd = %s);", login, password);
            pqxx::nontransaction NTO(postgres);
            pqxx::result rslt(NTO.exec(sql_search));
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