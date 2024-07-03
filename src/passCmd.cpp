#include "ft_irc.hpp"

/*## This is CAP order management ##*/

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

/*## This is PASS order management ##*/


void    passCmd(std::string const & msg, Server & Server, int const & fd)
{
    if (!msg.empty())
    {
        if (msg == Server.getPassword())
            Server._clients[fd].setPass();
        else
            SendMsg::ERR_PASSWDMISMATCH(fd);
    }
    else
        SendMsg::ERR_NEEDMOREPARAMS("PASS", fd);
    return ;
}

/*## This is NICK order management ##*/

static bool inUseNickName(std::string const & msg, Server & Server)
{
    std::map<int, Client>::iterator    it;

    for (it = Server._clients.begin(); it != Server._clients.end(); it++)
    {
        if (it->second.getNick() == msg)
            return true;
    }
    return false;
}

static bool notValidNick(std::string const & nick)
{
    if (nick[0] == '$' || nick[0] == ':' || nick[0] == '#' || nick[0] == '&')
        return true;
    else if (nick.find(' ') != std::string::npos || nick.find(',') != std::string::npos \
            || nick.find('*') != std::string::npos || nick.find('?') != std::string::npos \
            || nick.find('!') != std::string::npos || nick.find('@') != std::string::npos \
            || nick.find('.') != std::string::npos)
        return true;
    return false;
}

void    nickCmd(std::string const & msg, Server & Server, int const & fd)
{
    if (Server._clients[fd].getPass() == false)
        SendMsg::ERR_ALREADYREGISTERED(fd);
    else if (!msg.empty())
    {
        if (inUseNickName(msg, Server) == true)
            SendMsg::ERR_NICKNAMEINUSE(msg, fd);
        else if (notValidNick(msg) == true)
            SendMsg::ERR_ERRONEUSNICKNAME(msg, fd);
        else
            Server._clients[fd].setNick(msg);
    }
    else
        SendMsg::ERR_NONICKNAMEGIVEN(fd);
}

/*## This is USER order management ##*/

void    userCmd(std::string const & msg, Server & Server, int const & fd)
{
    if (!Server._clients[fd].getUser().empty())
        SendMsg::ERR_ALREADYREGISTERED(fd);
    else if (Server._clients[fd].getNick().empty() == true)
        SendMsg::ERR_ALREADYREGISTERED(fd);
    else if (!msg.empty())
    {
        Server._clients[fd].setUser(msg);
        SendMsg::RPL_WELCOME(Server, fd);
        SendMsg::RPL_YOURHOST(Server, fd);
        SendMsg::RPL_CREATED(Server, fd);
        SendMsg::RPL_MYINFO(Server, fd);
    }
    else
        SendMsg::ERR_NEEDMOREPARAMS("USER", fd);
}

/*## This is PING order management ##*/

void    pingCmd(std::string const & pMsg, int const & fd)
{
    if (!pMsg.empty())
    {
        std::string msg = "PONG " + pMsg + "\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
    }
    else
        SendMsg::ERR_NEEDMOREPARAMS("PING", fd);
}

/*## This is PONG order management ##*/

void    pongCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    std::string tmp = ':' + Server.getPingMsg();

    if (pMsg.empty())
        Server._clients.erase(fd); //send quit msg
    else if (pMsg == tmp)
        Server._clients[fd].setWaitingForPong(true);
    else if (pMsg == Server.getPingMsg())
        Server._clients[fd].setWaitingForPong(true);
}

/*## This is QUIT order management ##*/

void    quitCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    /*modifier et mettre la map client en paramettre et non le server*/
    /*check les arguments*/
    std::string msg;

    if (pMsg.empty())
        msg = ":" + Server._clients[fd].getNick() + "!" + Server._clients[fd].getUser() + "@host QUIT :leaving\r\n";
    else
        msg = ":" + Server._clients[fd].getNick() + "!" + Server._clients[fd].getUser() + "@host QUIT :" + pMsg + "\r\n";
    for (std::map<int, Client>::iterator it = Server._clients.begin(); it != Server._clients.end(); it++)
    {
        if (fd != it->second.getFd())
            SendMsg::QUIT(msg, it->second.getFd());
    }    
    Server._clients.erase(fd);
    return ;
}

/*## This is JOIN order management ##*/

std::vector<std::string>   splitNameChannel(std::string const & line)
{
    std::string                 name;
    std::vector<std::string>    nameChannels;

    for (int i = 0; line[i]; i++)
    {
        if (line[i] == '#')
        {
            while (line[i] != ' ' && line[i] != ',' && line[i])
                name += line[i++];
            nameChannels.push_back(name);
            name.clear();
        }
    }
    return nameChannels;
}

void    joinCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    std::vector<std::string>                    nameChannels;
    std::map<std::string, Channel>::iterator    channelIt;

    nameChannels = splitNameChannel(pMsg);

    for (std::vector<std::string>::iterator it = nameChannels.begin(); it != nameChannels.end(); it++)
    {
        channelIt = Server._channels.find(*it);
        if (channelIt == Server._channels.end())
        {
            Channel newChannel(*it);
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
        else
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
    }
}

/*## This is TOPIC order management ##*/

void    topicCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    int         pos = pMsg.find(' ');
    std::string channel = pMsg.substr(0, pos);
    std::string topic = pMsg.substr(pos + 2, std::string::npos);

    Server._channels[channel].setTopic(topic);
    //envoie du topic q tout les clients du channel
    SendMsg::RPL_TOPIC(Server._channels[channel], Server, fd);
    return ;
}

/*## This is PRIVMSG order management ##*/

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
    pMsg = ":" + Server._clients[fd].getNick() + "!~" + Server._clients[fd].getUser() + "@" + Server._clients[fd].getUser() + " PRIVMSG " + chan + " " + pMsg;
    
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
            send(it->first, pMsg.c_str(), pMsg.size(), 0);
    }
    return ;
}

void    sendClient(std::string pMsg, std::string & client, Server & Server, int const & fd)
{
    bool    ok = false;
    pMsg = ":" + Server._clients[fd].getNick() + "!~" + Server._clients[fd].getUser() + "@" + Server._clients[fd].getUser() + " PRIVMSG " + client + " " + pMsg;

    for (std::map<int, Client>::iterator it = Server._clients.begin(); it != Server._clients.end(); it++)
    {
        if (client == it->second.getNick())
        {
            send(it->first, pMsg.c_str(), pMsg.size(), 0);
            ok = true;
        }
    }
    if (!ok)
        SendMsg::ERR_NOSUCHNICK(client, Server, fd);
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
