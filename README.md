Before installing you'll need following packages installed:

libpqxx ver 7.7.4 (github: https://github.com/jtv/libpqxx/tree/7.7.4),
Postgres 14,
libpq (installs with Postgres),
C++17.

Project assemles with "make".

In "sql tables" you can find table schema for Users and one insert line.

For now "calc" command doesn't work properly. It cannot parse math expressions, but decreases tokens for operation if user has it.
Commands like "login", "password" and "logout" work as expected.

After compiling, server will get following usage:
./server <port>

And client:
./client <address> <port>
