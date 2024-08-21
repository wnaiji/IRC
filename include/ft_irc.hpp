#ifndef FT_IRC_HPP
# define FT_IRC_HPP

# include <cstdlib>
# include <cerrno>
# include <iostream>

# include "SendMsg.hpp"
# include "Server.hpp"

# define PORT_MIN 1024
# define PORT_MAX 65535

#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"

#define COUT_RED(x)     std::cerr << RED << x << RESET << std::endl
#define COUT_GREEN(x)   std::cout << GREEN << x << RESET << std::endl
#define COUT_YELLOW(x)  std::cout << YELLOW << x << RESET << std::endl
#define COUT_BLUE(x)    std::cout << BLUE << x << RESET << std::endl
#define COUT_MAGENTA(x) std::cout << MAGENTA << x << RESET << std::endl
#define COUT_CYAN(x)    std::cout << CYAN << x << RESET << std::endl
#define COUT_WHITE(x)   std::cout << WHITE << x << RESET << std::endl


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
void    capCmd(std::string const & msg, Server & Server, int const & fd);
void    passCmd(std::string const & msg, Server & Server, int const & fd);
void    nickCmd(std::string const & msg, Server & Server, int const & fd);
void    userCmd(std::string const & msg, Server & Server, int const & fd);
void    pingCmd(std::string const & pMsg, int const & fd);
void    pongCmd(std::string const & pMsg, Server & Server, int const & fd);
void    quitCmd(std::string const & pMsg, Server & Server, int const & fd);
void    joinCmd(std::string const & pMsg, Server & Server, int const & fd);
void    topicCmd(std::string const & pMsg, Server & Server, int const & fd);
void    privmsgCmd(std::string const & pMsg, Server & Server, int const & fd);
void    modeCmd(std::string const & pMsg, Server & Server, int const & fd);
void    inviteCmd(std::string const & pMsg, Server & Server, int const & fd);
void    partCmd(std::string const & pMsg, Server & Server, int const & fd);
void    kickCmd(std::string const & pMsg, Server & Server, int const & fd);
void    botCmd(std::string const & pMsg, Server & Server, int const & fd);

#endif
