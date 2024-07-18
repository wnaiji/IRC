#include "ft_irc.hpp"

bool    privileges(std::string const & channel, Server & Server, int const & fd)
{
    (void)fd;
    for (std::vector<std::string>::const_iterator it = Server._channels[channel].getNameAdmin().begin(); it != Server._channels[channel].getNameAdmin().end(); it++)
    {
        if (*it == Server._clients[fd].getNick())
            return true;
    }
    return false;
}

std::vector<std::string>   splitArg(std::string const & mode, std::string const & line)
{
    std::string                 dest;
    std::vector<std::string>    listDest;
    int                         j = 0;

    for (int i = 0; mode[i]; i++)
    {
        if (mode[i] != '+' && mode[i] != '-')
        {
            if (mode[i] == 'i' || mode[i] == 't')
                listDest.push_back(dest);
            else
            {
                while (line[j] != ' ' && line[j])
                    dest += line[j++];
                listDest.push_back(dest);
                dest.clear();
            }
            if (line[j] == ' ')
                j++;
        }
        if (mode[i] == '-' && (mode[i + 1] == 'l' || mode[i + 1] == 'k'))
        {
            listDest.push_back(dest);
            i += 2;
        }
    }
    return listDest;
}

void    handleModeCommand(std::string const & pMode, std::string const & channel, Server & Server, int const & fd)
{
    bool                        adding = true;
    unsigned long               pos = pMode.find(' ');
    std::string                 mode = pMode.substr(0, pos);
    std::string                 line;

    if (pos != std::string::npos)
        line = pMode.substr(pos + 1, std::string::npos);

    std::vector<std::string>    arg = splitArg(mode, line);
    std::vector<std::string>::iterator  it = arg.begin();
    for (unsigned long i = 0; i < mode.size(); i++)
    {
        if (mode[i] == '+')
            adding = true;
        else if (mode[i] == '-')
            adding = false;
        else
        {
            if (adding)
                Server._channels[channel].addMode(mode[i], *it, Server, fd);
            else
                Server._channels[channel].removeMode(mode[i], *it, Server, fd);
            it++;
        }
    }
    return ;
}

void    modeCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    unsigned long       pos = pMsg.find(' ');
    std::string const   channel = pMsg.substr(0, pos);
    std::string         mode;

    if (pos != std::string::npos)
        mode = pMsg.substr(pos + 1, std::string::npos);

    if (Server._channels.find(channel) == Server._channels.end())
        SendMsg::ERR_NOSUCHCHANNEL(channel, Server, fd);
    else if (pMsg.size() == channel.size())
        SendMsg::RPL_CHANNELMODEIS(Server._channels[channel], Server, fd);
    else if (privileges(channel, Server, fd) == false)
        SendMsg::ERR_CHANOPRIVSNEEDED(channel, Server, fd);
    else
        handleModeCommand(mode, channel, Server, fd);
}
