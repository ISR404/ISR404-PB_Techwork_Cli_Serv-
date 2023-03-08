#ifndef SRVCMD_HPP_INCLUDED
#define SRVCMD_HPP_INCLUDED

#include <pqxx/pqxx>
#include <iostream>
#include <mutex>

#define CMD_LOGIN "login"
#define CMD_PASSWD "password"
#define CMD_LOGOUT "logout"
#define CMD_CALC "calc"

#define STATUS_INV -1
#define STATUS_LOG 0
#define STATUS_PASSWD 1
#define STATUS_LOGOUT 2
#define STATUS_CALC 3

/* 
  Returns values from -1 to 3, where 
  -1 (STATUS_INV) - invalid command
   0 (STATUS_LOG) - login command,
   1 (STATUS_PASSWD) - password command,
   2 (STATUS_LOGOUT) - logout command,
   3 (STATUS_CALC) - calc command.
*/
int GetCommandStatus(const char* msg);

/*
  Extracts <username> string from command "login <username>"
*/
std::string GetLogin(std::string msg);

/*
  Extracts <password> string from command "password <password>"
*/
std::string GetPassword(std::string msg);

/*
  Extracts <expression> string from command "calc <expression>"
*/
std::string GetCalcExpression(std::string msg);

/*
  Searches existing account in postgres database.
  <login> - user's username,
  <password> - user's password,
  <&db_conn_str> - constant reference to connection string that must be initialized in FillLogDataDB function.
*/
bool IsFoundAuthDB(std::string login, std::string password, const std::string& db_conn_str);

/*
  Checks if user has enough tokens to process "calc" command.
  <login> - user's username,
  <password> - user's password,
  <&db_conn_str> - constant reference to connection string that must be initialized in FillLogDataDB function,
  <&tokens> - reference to an integer that will contain user's remaining tokens (Needs for further string formatting in DecreaseAccountTokens function). 
*/
bool IsEnoughTokensDB(std::string login, std::string password, const std::string& db_conn_str, int& tokens);

/*
  Decreases account tokens for "calc operations".
  <login> - user's username,
  <password> - user's password,
  <&db_conn_str> - constant reference to connection string that must be initialized in FillLogDataDB function,
  <tokens> - integer value that must be initialized in IsFoundAuthDB function. Represents remaining account tokens for "calc" operations
*/
void DecreaseAccountTokens(std::string login, std::string password, const std::string& db_conn_str, int tokens);

/*
  Main function that handles connection between client and server. Must be called from new thread to orginize async sessions.
  <connect_sock> - socket file descriptor that must be initialized from Accept() function,
  <active_conn_slot> - pointer to integer in array of connections. When connection closes, value in pointer sets to 0, that means "slot is available",
  <&db_conn_str> - reference to database connection string, initialized in FillLogDataDB function.
*/
void ServerConnectHandler(const int connect_sock, int* active_conn_slot, const std::string& db_conn_str);

/*
  Function that creates SQL string with dynamic log data, such as database name, login and password.
  Result connection writes in <&conn_string> reference. This reference uses in functions that need database connection.
*/
void FillLogDataDB(std::string &conn_string);

#endif