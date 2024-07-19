#include "ft_irc.hpp"

void    quitCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    std::string msg;
    int         i = 0;
    if (pMsg.empty())
        msg = ":" + Server._clients[fd].getNick() + "!" + Server._clients[fd].getNick() + "@" + Server._clients[fd].getNick() + " QUIT :leaving\r\n";
    else
        msg = ":" + Server._clients[fd].getNick() + "!" + Server._clients[fd].getNick() + "@" + Server._clients[fd].getNick() + " QUIT " + pMsg + "\r\n";
    for (std::map<int, Client>::iterator it = Server._clients.begin(); it != Server._clients.end(); it++)
    {
        if (fd != it->second.getFd())
            SendMsg::QUIT(msg, it->second.getFd());
    }
    for (std::map<std::string, Channel>::iterator it = Server._channels.begin(); it != Server._channels.end(); it++)
    {
        for (std::map<int, Client *>::iterator it2 = it->second._clients.begin(); it2 != it->second._clients.end(); it2++)
        {
            if (Server._clients[fd].getNick() == it2->second->getNick())
            {
                Server._channels[it->first]._clients.erase(fd);
                if (Server._channels[it->first]._clients.size() == 0)
                    Server._channels.erase(it->first);
                i = 1;
                break ;
            }
        }
        if (i == 1)
            break ;
    }
    Server._clients.erase(fd);
    close(fd);
    return ;
}
