#ifndef FT_IRC_HPP
# define FT_IRC_HPP

# include <cstdlib> // for strtol
# include <cerrno> // for errno
# include <iostream>

# include "SendMsg.hpp"
# include "Server.hpp"

# define PORT_MIN 1024
# define PORT_MAX 65535

enum e_exit_error
{
    MAIN_ERROR_ARGS = 1,
    GET_PORT_ERROR_PARS,
    GET_PORT_ERROR_LIMIT,
    GET_PORT_ERROR_RANGE,
};

using std::cerr;
using std::endl;

void    manageCmd(std::string const & line, Server & Server, int const & fd);
void    capCmd(Server & Server, int const & fd);
void    passCmd(std::string const & msg, Server & Server, int const & fd);
void    nickCmd(std::string const & msg, Server & Server, int const & fd);
void    userCmd(std::string const & msg, Server & Server, int const & fd);

#endif
