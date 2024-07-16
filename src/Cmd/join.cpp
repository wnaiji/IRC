#include "ft_irc.hpp"

std::vector<std::string>   splitNameChannel(std::string const & line)
{
    std::string                 name;
    std::vector<std::string>    nameChannels;

    for (int i = 0; line[i]; i++)
    {
        if (line[i] == '#')
        {
            while (line[i] != ',' && line[i])
                name += line[i++];
            nameChannels.push_back(name);
            name.clear();
        }
    }
    return nameChannels;
}

std::vector<std::string>    splitKeyChannels(std::string const & line)
{
    std::string                 key;
    std::vector<std::string>    keyChannels;

    for (int i = 0; line[i]; i++)
    {
        if (line[i] == '#')
        {
            while (line[i] != ',' && line[i])
                key += line[i++];
            keyChannels.push_back(key);
            key.clear();
        }
    }
    return keyChannels;
}

static bool    invite(std::string const & channel, Server & Server, int const & fd)
{
    if (Server._channels[channel].getInviteStatus() == true)
    {
        std::vector<std::string>    inviteName = Server._channels[channel].getInvitName();
        for (std::vector<std::string>::iterator it = inviteName.begin(); it != inviteName.end(); it++)
        {
            if (*it == Server._clients[fd].getNick())
                return true;
        }
        return false;
    }
    return true;
}

static bool    limmit(std::string const & channel, Server & Server)
{
    if (Server._channels[channel].getLimit() > 0)
    {
        if (Server._channels[channel]._clients.size() >= static_cast<unsigned long>(Server._channels[channel].getLimit()))
            return false;
    }
    return true;
}

static bool     password(std::string const & channel, std::string const & password, Server & Server)
{
    if (Server._channels[channel].getKeyStatus() == true)
    {
        if (Server._channels[channel].getPassWord() == password)
            return true;
    }
    return true;
}

void    joinCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    std::vector<std::string>                    nameChannels;
    std::vector<std::string>                    keyChannels;
    std::map<std::string, Channel>::iterator    channelIt;
    unsigned long                               pos = pMsg.find(' ');

    nameChannels = splitNameChannel(pMsg.substr(0, pos));
    if (pos != std::string::npos)
        keyChannels = splitKeyChannels(pMsg.substr(pos + 1, std::string::npos));
    pos = 0;
    for (std::vector<std::string>::iterator it = nameChannels.begin(); it != nameChannels.end(); it++)
    {
        channelIt = Server._channels.find(*it);
        if (channelIt == Server._channels.end())
        {
            Channel newChannel(*it);
            newChannel.setNameAdmin("+o", Server._clients[fd].getNick());
            Server._channels.insert(std::make_pair(*it, newChannel));
            Server._channels[*it]._clients[fd] = &Server._clients[fd];
            SendMsg::JOINS(*it, Server, fd);
            if (Server._channels[*it].getTopic().empty())
                SendMsg::RPL_NOTOPIC(Server._channels[*it], Server, fd);
            else
                SendMsg::RPL_TOPIC(Server._channels[*it], Server, fd);
            SendMsg::RPL_NAMREPLY(Server._channels[*it], Server, fd);
            SendMsg::RPL_ENDOFNAMES(Server._channels[*it], Server, fd);
        }
        else if (invite(*it, Server, fd) && limmit(*it, Server) && password(*it, keyChannels[pos], Server))
        {
            Server._channels[*it]._clients[fd] = &Server._clients[fd];
            SendMsg::JOINS(*it, Server, fd);
            if (Server._channels[*it].getTopic().empty())
                SendMsg::RPL_NOTOPIC(Server._channels[*it], Server, fd);
            else
                SendMsg::RPL_TOPIC(Server._channels[*it], Server, fd);
            SendMsg::RPL_NAMREPLY(Server._channels[*it], Server, fd);
            SendMsg::RPL_ENDOFNAMES(Server._channels[*it], Server, fd);
        }
        pos++;
    }
}
