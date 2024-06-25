#include "ft_irc.hpp"

/*  gestion de la commande pass et de la commande CAP                   *
*   la commande CAP sera tout simplement mis sous forme de bool         *
*   dans la class Client une fois passe on ira a la commande PASS       *
*   il y aura donc un parsing pour verifier la commande PASS puis le    *
*   mot de pass                                                         */

/*## This is CAP order management ##*/

void    capCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    if (pMsg.empty() || pMsg == "LS")
    {
        Server._clients[fd].setCap();
        SendMsg::CAP(1, fd);
    }
    else
        SendMsg::CAP(0, fd);
    return ;
}

/*## This is PASS order management ##*/


void    passCmd(std::string const & msg, Server & Server, int const & fd)
{
    if (!msg.empty())
    {
        if (msg == Server.getPassword())
            Server._clients[fd].setPass();
        else
            SendMsg::ERR_PASSWDMISMATCH(fd);
    }
    else
        SendMsg::ERR_NEEDMOREPARAMS("PASS", fd);
    return ;
}

/*## This is NICK order management ##*/

static bool inUseNickName(std::string const & msg, Server & Server)
{
    std::map<int, Client>::iterator    it;

    for (it = Server._clients.begin(); it != Server._clients.end(); it++)
    {
        if (it->second.getNick() == msg)
            return true;
    }
    return false;
}

static bool notValidNick(std::string const & nick)
{
    if (nick[0] == '$' || nick[0] == ':' || nick[0] == '#' || nick[0] == '&')
        return true;
    else if (nick.find(' ') != std::string::npos || nick.find(',') != std::string::npos \
            || nick.find('*') != std::string::npos || nick.find('?') != std::string::npos \
            || nick.find('!') != std::string::npos || nick.find('@') != std::string::npos \
            || nick.find('.') != std::string::npos)
        return true;
    return false;
}

void    nickCmd(std::string const & msg, Server & Server, int const & fd)
{
    if (Server._clients[fd].getPass() == false)
        SendMsg::ERR_ALREADYREGISTERED(fd);
    else if (!msg.empty())
    {
        if (inUseNickName(msg, Server) == true)
            SendMsg::ERR_NICKNAMEINUSE(msg, fd);
        else if (notValidNick(msg) == true)
            SendMsg::ERR_ERRONEUSNICKNAME(msg, fd);
        else
            Server._clients[fd].setNick(msg);
    }
    else
        SendMsg::ERR_NONICKNAMEGIVEN(fd);
}

/*## This is USER order management ##*/

void    userCmd(std::string const & msg, Server & Server, int const & fd)
{
    if (!Server._clients[fd].getUser().empty())
        SendMsg::ERR_ALREADYREGISTERED(fd);
    else if (Server._clients[fd].getNick().empty() == true)
        SendMsg::ERR_ALREADYREGISTERED(fd);
    else if (!msg.empty())
    {
        Server._clients[fd].setUser(msg);
        SendMsg::RPL_WELCOME(Server, fd);
        SendMsg::RPL_YOURHOST(Server, fd);
        SendMsg::RPL_CREATED(Server, fd);
        SendMsg::RPL_MYINFO(Server, fd);
    }
    else
        SendMsg::ERR_NEEDMOREPARAMS("USER", fd);
}

/*## This is PING order management ##*/

void    pingCmd(std::string const & pMsg, int const & fd)
{
    if (!pMsg.empty())
    {
        std::string msg = "PONG " + pMsg + "\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
    }
    else
        SendMsg::ERR_NEEDMOREPARAMS("PING", fd);
}

/*## This is PONG order management ##*/

void    pongCmd(std::string const & pMsg, Server & Server,int const & fd)
{
    std::string tmp = ':' + Server.getPingMsg();

    if (pMsg.empty())
        Server._clients.erase(fd); //send quit msg
    else if (pMsg == tmp)
        Server._clients[fd].setWaitingForPong(true);
    else if (pMsg == Server.getPingMsg())
        Server._clients[fd].setWaitingForPong(true);
}
