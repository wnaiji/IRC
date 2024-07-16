#include "ft_irc.hpp"

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

static void    sendToUserChan(std::string const & pMsg, Server & Server, int const & fd)
{
    for (std::map<std::string, Channel>::iterator it = Server._channels.begin(); it != Server._channels.end(); it++)
    {
        for (std::map<int, Client *>::iterator it2 = it->second._clients.begin(); it2 != it->second._clients.end(); it2++)
        {
            if (Server._clients[fd].getNick() == it2->second->getNick())
            {
                for (it2 = it->second._clients.begin(); it2 != it->second._clients.end(); it2++)
                {
                        std::string msg = ":" + Server._clients[fd].getNick() + "!~" + it2->second->getNick() + "@localhost NICK :" + pMsg + "\r\n";
                        send(it2->second->getFd(), msg.c_str(), msg.size(), 0);
                }
                break ;
            }
        }
    }
}

void    nickCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    if (Server._clients[fd].getPass() == false)
        SendMsg::ERR_ALREADYREGISTERED(fd);
    else if (Server._clients[fd].getNick().empty() && !pMsg.empty())
    {
        if (inUseNickName(pMsg, Server) == true)
            SendMsg::ERR_NICKNAMEINUSE(pMsg, fd);
        else if (notValidNick(pMsg) == true)
            SendMsg::ERR_ERRONEUSNICKNAME(pMsg, fd);
        else
            Server._clients[fd].setNick(pMsg);
    }
    else if (inUseNickName(pMsg, Server) == true)
        SendMsg::ERR_NICKNAMEINUSE(pMsg, fd);
    else if (notValidNick(pMsg) == true)
        SendMsg::ERR_ERRONEUSNICKNAME(pMsg, fd);
    else if (!pMsg.empty())
    {
        sendToUserChan(pMsg, Server, fd);
        Server._clients[fd].setNick(pMsg);
    }
    else
        SendMsg::ERR_NONICKNAMEGIVEN(fd);
}
