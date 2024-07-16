#include "ft_irc.hpp"

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
