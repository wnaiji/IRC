#ifndef SENDMSG_HPP
# define SENDMSG_HPP

# include "ft_irc.hpp"
# include <ctime>

class Server;

class SendMsg
{
public:
    static void PING(std::string const & pMsg, int const & fd);
    static void CAP(int const & index, int const & fd);
    static void QUIT(std::string const & cmd, int const & fd);

    static void RPL_WELCOME(Server & Server, int const & fd);
    static void RPL_YOURHOST(Server & Server, int const & fd);
    static void RPL_CREATED(Server & Server, int const & fd);
    static void RPL_MYINFO(Server & Server, int const fd);

    static void ERR_PASSWDMISMATCH(int const & fd);
    static void ERR_NICKNAMEINUSE(std::string const & nick, int const & fd);
    static void ERR_NONICKNAMEGIVEN(int const & fd);
    static void ERR_ERRONEUSNICKNAME(std::string const & nick, int const & fd);
    static void ERR_NEEDMOREPARAMS(std::string const & cmd, int const & fd);
    static void ERR_ALREADYREGISTERED(int const & fd);
};

#endif