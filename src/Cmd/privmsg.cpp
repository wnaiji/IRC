#include "ft_irc.hpp"

std::vector<std::string>   splitDest(std::string const & line)
{
    std::string                 dest;
    std::vector<std::string>    listDest;

    for (int i = 0; line[i]; i++)
    {
        if (line[i] == '#')
        {
            while (line[i] != ' ' && line[i] != ',' && line[i])
                dest += line[i++];
            listDest.push_back(dest);
            dest.clear();
        }
        else
        {
            while (line[i] && line[i] != ' ' && line[i] != ',')
                dest += line[i++];
            listDest.push_back(dest);
            dest.clear();
        }
    }
    return listDest;
}

void    sendChannel(std::string pMsg, std::string & chan, Server & Server, int const & fd)
{
    bool    ok = false;
    std::string msg = ":" + Server._clients[fd].getNick() + "!" + Server._clients[fd].getNick() + "@" + Server._clients[fd].getNick() + " PRIVMSG " + chan + " :" + pMsg;

    for (std::map<int, Client *>::iterator it = Server._channels[chan]._clients.begin(); it != Server._channels[chan]._clients.end(); it++)
    {
        if (it->second->getNick() == Server._clients[fd].getNick())
            ok = true;
    }
    if (!ok)
        SendMsg::ERR_CANNOTSENDTOCHAN(chan, Server, fd);
    else
    {
        for (std::map<int, Client *>::iterator it = Server._channels[chan]._clients.begin(); it != Server._channels[chan]._clients.end(); it++)
        {
            if (it->first != fd)
                send(it->second->getFd(), msg.c_str(), msg.size(), 0);
        }
    }
    return ;
}

void    sendClient(std::string pMsg, std::string & client, Server & Server, int const & fd)
{
    bool    ok = false;
    pMsg = ":" + Server._clients[fd].getNick() + "!" + Server._clients[fd].getNick() + "@" + Server._clients[fd].getNick() + " PRIVMSG " + client + " :" + pMsg;

    for (std::map<int, Client>::iterator it = Server._clients.begin(); it != Server._clients.end(); it++)
    {
        if (client == it->second.getNick())
        {
            send(it->first, pMsg.c_str(), pMsg.size(), 0);
            ok = true;
        }
    }
    //if (!ok)
    //    SendMsg::ERR_NOSUCHNICK(client, Server, fd);
    return ;
}

void    privmsgCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    int                         pos = pMsg.find(":");
    std::string                 dest = pMsg.substr(0, pos - 1);
    std::string                 msg = pMsg.substr(pos + 1, std::string::npos) + "\r\n";
    std::vector<std::string>    listDest = splitDest(dest);

    for (std::vector<string>::iterator it = listDest.begin(); it != listDest.end(); it++)
    {
        if ((*it)[0] == '#')
            sendChannel(msg, *it, Server, fd);
        else
            sendClient(msg, *it, Server, fd);
    }
}