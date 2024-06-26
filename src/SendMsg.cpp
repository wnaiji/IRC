#include "SendMsg.hpp"

void    SendMsg::PING(std::string const & pMsg, int const & fd)
{
    std::string msg = "PING " + pMsg + "\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

void    SendMsg::CAP(int const & index, int const & fd)
{
    std::string msg;

    if (index == 1)
        msg = ":42serv CAP * LS :\r\n";
    else
        std::string msg = ":42serv CAP * ACK :multi-prefix sasl\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

void    SendMsg::QUIT(std::string const & cmd, int const & fd)
{
    send(fd, cmd.c_str(), cmd.size(), 0);
    return ;
}

void    SendMsg::RPL_WELCOME(Server & Server, int const & fd)
{
    std::string msg = ":42serv 001 " + Server._clients[fd].getNick() + " :Welcome to the Internet Relay Network, " + Server._clients[fd].getNick() + "!" + Server._clients[fd].getUser() + "@" + Server._clients[fd].getUser() + "\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

void    SendMsg::RPL_YOURHOST(Server & Server, int const & fd)
{
    std::string msg = ":42serv 002 " + Server._clients[fd].getNick() + " :Your host is 42serv, running version 4.2\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

void    SendMsg::RPL_CREATED(Server & Server, int const & fd)
{
    std::string msg = ":42serv 003 " + Server._clients[fd].getNick() + " :This server was created today\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

void    SendMsg::RPL_MYINFO(Server & Server, int const fd)
{
    std::string msg = ":42serv 004 " + Server._clients[fd].getNick() + " 42serv 4.2 o o\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

void    SendMsg::ERR_PASSWDMISMATCH(int const & fd)
{
    std::string msg = ":42serv 464 <client> :Password incorrect\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

void    SendMsg::ERR_NICKNAMEINUSE(std::string const & nick, int const & fd)
{
    std::string msg = ":42serv 433 <client> " + nick + " :Nickname is already in use\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

void    SendMsg::ERR_NONICKNAMEGIVEN(int const & fd)
{
    std::string msg = ":42serv 431 <client> :No nickname given\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

void    SendMsg::ERR_ERRONEUSNICKNAME(std::string const & nick, int const & fd)
{
    std::string msg = ":42serv 432 <client> " + nick + " :Erroneus nickname\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

void    SendMsg::ERR_NEEDMOREPARAMS(std::string const & cmd, int const & fd)
{
    std::string msg = ":42serv 461 <client> " + cmd + " :Not enough parameters\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

void    SendMsg::ERR_ALREADYREGISTERED(int const & fd)
{
    std::string msg = ":42serv 462 <client> :You may not reregister\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return ;
}

