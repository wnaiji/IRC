#include "ft_irc.hpp"

static bool onChannel(std::string const & nick, Channel const & Channel)
{
    for (std::map<int, Client *>::const_iterator it = Channel._clients.begin(); it != Channel._clients.end(); it++)
    {
        if (it->second->getNick() == nick)
            return true;
    }
    return false;
}

static int fdDest(std::string const & nick, Server & Server)
{
    for (std::map<int, Client>::iterator it = Server._clients.begin(); it != Server._clients.end(); it++)
    {
        if (nick == it->second.getNick())
            return it->first;
    }
    return 2;
}

void    inviteCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    int pos = pMsg.find(' ');
    std::string nick = pMsg.substr(0, pos);
    std::string channel = pMsg.substr(pos + 1, std::string::npos);

    if (Server._channels.find(channel) == Server._channels.end())
        SendMsg::ERR_NOSUCHCHANNEL(channel, Server, fd);
    else if (onChannel(Server._clients[fd].getNick(), Server._channels[channel]) == false)
        SendMsg::ERR_NOTONCHANNEL(channel, Server, fd);
    else if (onChannel(nick, Server._channels[channel]) == true)
        SendMsg::ERR_USERONCHANNEL(channel, nick, Server, fd);
    else
    {
        Server._channels[channel].setInviteName("+i", nick);
        SendMsg::RPL_INVITING(channel, nick, Server, fd);
        std::string msg = ":" + Server._clients[fd].getNick() + "!" + Server._clients[fd].getNick() + "@" + Server._clients[fd].getNick() + " INVITE " + nick + " :" + channel + "\r\n";
        send(fdDest(nick, Server), msg.c_str(), msg.size(), 0);
    }
}
