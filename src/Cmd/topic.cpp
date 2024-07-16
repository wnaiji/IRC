#include "ft_irc.hpp"

bool    yourAreAdmin(std::vector<std::string> const & list, std::string const & name)
{
    for (std::vector<std::string>::const_iterator it = list.begin(); it != list.end(); it++)
    {
        if (*it == name)
            return true;
    }
    return false;
}

void    topicCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    int         pos = pMsg.find(' ');
    std::string channel = pMsg.substr(0, pos);
    std::string topic = pMsg.substr(pos + 2, std::string::npos);

    if (channel.empty())
        SendMsg::ERR_NEEDMOREPARAMS("TOPIC", fd);
    else if (Server._channels.find(channel) == Server._channels.end())
        SendMsg::ERR_NOSUCHCHANNEL(channel, Server, fd);
    else if (Server._channels[channel]._clients.find(fd) == Server._channels[channel]._clients.end())
        SendMsg::ERR_NOTONCHANNEL(channel, Server, fd);
    else if (yourAreAdmin(Server._channels[channel].getNameAdmin(), Server._clients[fd].getNick()) == false)
        SendMsg::ERR_CHANOPRIVSNEEDED(channel, Server, fd);
    else
    {
        Server._channels[channel].setTopic(topic);
        for (std::map<int, Client *>::iterator it = Server._channels[channel]._clients.begin(); it != Server._channels[channel]._clients.end(); it++)
            SendMsg::RPL_TOPIC(Server._channels[channel], Server, it->first);
    }
    return ;
}
