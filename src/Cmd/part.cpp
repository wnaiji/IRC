#include "ft_irc.hpp"

static std::vector<std::string>    splitChannel(std::string const & channel)
{
    std::vector<std::string>    listChannel;
    std::string                 newChan;

    for (int i = 0; channel[i]; i++)
    {
        if (channel[i] == '#')
        {
            while (channel[i] && channel[i] != ',')
                newChan += channel[i++];
            listChannel.push_back(newChan);
            newChan.clear();
        }
    }
    return listChannel;
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

void    partCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    if (pMsg.empty())
        SendMsg::ERR_NEEDMOREPARAMS("PART", fd);
    else
    {
        int pos = pMsg.find(' ');
        std::vector<std::string> channels = splitChannel(pMsg.substr(0, pos));

        for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            if (Server._channels.find(*it) == Server._channels.end())
                SendMsg::ERR_NOSUCHCHANNEL(*it, Server, fd);
            else if (onChannel(Server._clients[fd].getNick(), Server._channels[*it]) == false)
                SendMsg::ERR_NOTONCHANNEL(*it, Server, fd);
            else
            {
                SendMsg::PART(*it, Server, fd);
                Server._channels[*it]._clients.erase(fd);
                if (Server._channels[*it]._clients.size() == 0)
                    Server._channels.erase(*it);
            }
        }
    }
}
