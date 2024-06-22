#include "SendMsg.hpp"

void    SendMsg::RPL_WELCOME(Server & Server, int const & fd)
{
    std::string msg = "001 <client> :Welcome to the 42 Network, " + Server._clients[fd].getNick() + "\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

void    SendMsg::RPL_YOURHOST(Server & Server, int const & fd)
{
    (void)Server;
    (void)fd;
}

void    SendMsg::RPL_CREATED(Server & Server, int const & fd)
{
    (void)Server;
    (void)fd;
}

void    SendMsg::ERR_PASSWDMISMATCH(int const & fd)
{
    std::string msg = "464 <client> :Password incorrect\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

void    SendMsg::ERR_NICKNAMEINUSE(std::string const & nick, int const & fd)
{
    std::string msg = "433 <client> " + nick + " :Nickname is already in use\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

void    SendMsg::ERR_NONICKNAMEGIVEN(int const & fd)
{
    std::string msg = "431 <client> :No nickname given\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

void    SendMsg::ERR_ERRONEUSNICKNAME(std::string const & nick, int const & fd)
{
    std::string msg = "432 <client> " + nick + " :Erroneus nickname\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

void    SendMsg::ERR_NEEDMOREPARAMS(std::string const & cmd, int const & fd)
{
    std::string msg = "461 <client> " + cmd + " :Not enough parameters\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

void    SendMsg::ERR_ALREADYREGISTERED(int const & fd)
{
    std::string msg = "462 <client> :You may not reregister\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}
