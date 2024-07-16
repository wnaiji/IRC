#include "ft_irc.hpp"

void    pingCmd(std::string const & pMsg, int const & fd)
{
    std::string msg;

    if (!pMsg.empty())
    {
        if (pMsg.find(':') != std::string::npos)
            msg = "PONG " + pMsg + "\r\n";
        else
            msg = "PONG :" + pMsg + "\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
    }
    else
        SendMsg::ERR_NEEDMOREPARAMS("PING", fd);
}
