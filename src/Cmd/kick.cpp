#include "ft_irc.hpp"

static bool isOperator(std::string const & nick, Channel const & Channel)
{
    for (std::vector<std::string>::const_iterator it = Channel.getNameAdmin().begin(); it != Channel.getNameAdmin().end(); it++)
    {
        if (nick == *it)
            return true;
    }
    return false;
}

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

void    kickCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    int         pos = pMsg.find(' ');
    std::string channel = pMsg.substr(0, pos);
    std::string msg = pMsg.substr(pos + 1, std::string::npos);
    int         pos2 = msg.find(' ');
    std::string nick = msg.substr(0, pos2);

    if (channel.empty())
        SendMsg::ERR_NEEDMOREPARAMS("KICK", fd);
    else if (Server._channels.find(channel) == Server._channels.end())
        SendMsg::ERR_NOSUCHCHANNEL(channel, Server, fd);
    else if(isOperator(Server._clients[fd].getNick(), Server._channels[channel]) == false)
        SendMsg::ERR_CHANOPRIVSNEEDED(channel, Server, fd);
    else if (onChannel(Server._clients[fd].getNick(), Server._channels[channel]) == false)
        SendMsg::ERR_NOTONCHANNEL(channel, Server, fd);
    else
    {
        SendMsg::KICK(channel, nick, Server, fd);
        Server._channels[channel]._clients.erase(fdDest(nick, Server));
    }
}
