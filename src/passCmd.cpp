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
    std::string msg;

    if (pMsg.empty())
        msg = ":" + Server._clients[fd].getNick() + "!" + Server._clients[fd].getNick() + "@host QUIT :leaving\r\n";
    else
        msg = ":" + Server._clients[fd].getNick() + "!" + Server._clients[fd].getNick() + "@host QUIT " + pMsg + "\r\n";
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

/*## This is MODE order management ##*/

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

    std::cout << "33333333333333333333333" << std::endl;

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

                std::cout << "##########" << dest << "#" <<std::endl;
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
    std::cout << "============" << listDest.size() << "=" << std::endl;
    return listDest;
}

void    handleModeCommand(std::string const & pMode, std::string const & channel, Server & Server, int const & fd)
{
    bool                        adding = true;
    unsigned long               pos = pMode.find(' ');
    std::string                 mode = pMode.substr(0, pos);
    std::string                 line;
    std::cout << "222222222222222222222" << std::endl;

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
                Server._channels[channel].addMode(mode[i], *it++, Server, fd);
            else
                Server._channels[channel].removeMode(mode[i], *it++, Server, fd);
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

    std::cout << "1111111111111111111111" << channel << std::endl;

    if (Server._channels.find(channel) == Server._channels.end())
    {
        cout << "aaaaaaaaaaaaaaaa" << endl;
        SendMsg::ERR_NOSUCHCHANNEL(channel, Server, fd);
    }
    else if (pMsg.size() == channel.size())
    {
        cout << "bbbbbbbbbbbbbbbb" << endl;
        SendMsg::RPL_CHANNELMODEIS(Server._channels[channel].getName(), Server, fd);
    }
    else if (privileges(channel, Server, fd) == false)
    {
        cout << "cccccccccccccccc" << endl;
        SendMsg::ERR_CHANOPRIVSNEEDED(channel, Server, fd);
    }
    else
    {
        cout << "dddddddddddddddd" << endl;
        handleModeCommand(mode, channel, Server, fd);
    }
    std::cout << "ppppppppppppppp" << endl;

}
