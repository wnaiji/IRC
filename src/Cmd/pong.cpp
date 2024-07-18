#include "ft_irc.hpp"

void    pongCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    std::string tmp = ':' + Server.getPingMsg();

    if (pMsg.empty())
        Server._clients.erase(fd);
    else if (pMsg == tmp)
        Server._clients[fd].setWaitingForPong(true);
    else if (pMsg == Server.getPingMsg())
        Server._clients[fd].setWaitingForPong(true);
}
