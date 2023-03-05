#ifndef SRVCMD_HPP_INCLUDED
#define SRVCMD_HPP_INCLUDED

#include <pqxx/pqxx>
#include <iostream>

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
   0 (STATUS_LOG)- login command,
   1 (STATUS_PASSWD)- password command,
   2 (STATUS_LOGOUT)- logout command,
   3 (STATUS_CALC)- calc command.
*/
int GetCommandStatus(const char* msg);

std::string GetLogin(std::string msg);

std::string GetPassword(std::string msg);

bool IsFoundAuthDB(std::string login, std::string password);

#endif