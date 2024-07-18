#include "ft_irc.hpp"

static bool isCap(std::string const & cmd)
{
    return cmd == "CAP";
}

static bool isPass(std::string const & cmd)
{
    return cmd == "PASS";
}

static bool isNick(std::string const & cmd)
{
    return cmd == "NICK";
}

static bool isUser(std::string const & cmd)
{
    return cmd == "USER";
}

static bool isPing(std::string const & cmd)
{
    return cmd == "PING";
}

static bool isPong(std::string const & cmd)
{
    return cmd == "PONG";
}

static bool isQuit(std::string const & cmd)
{
    return cmd == "QUIT";
}

static bool isJoin(std::string const & cmd)
{
    return cmd == "JOIN";
}

static bool isTopic(std::string const & cmd)
{
    return cmd == "TOPIC";
}

static bool isPrivmsg(std::string const & cmd)
{
    return cmd == "PRIVMSG";
}

static bool isMode(std::string const & cmd)
{
    return cmd == "MODE";
}

static bool isInvite(std::string const & cmd)
{
    return cmd == "INVITE";
}

static bool isPart(std::string const & cmd)
{
    return cmd == "PART";
}

static bool isKick(std::string const & cmd)
{
    return cmd == "KICK";
}

void    manageCmd(std::string const & line, Server & Server, int const & fd)
{
    size_t      pos = line.find(' ');
    std::string cmd;
    std::string msg;
    bool        (*isCmd[])(std::string const &) = {isCap, isPass, isNick, isUser, isPing, \
                                                    isPong, isQuit, isJoin, isTopic, isPrivmsg, \
                                                    isMode, isInvite, isPart, isKick};
    std::cout << line << std::endl;
    if (pos != std::string::npos)
    {
        cmd = line.substr(0, pos);
        msg = line.substr(pos + 1, std::string::npos);
        if (msg[msg.size() - 1] == '\r' || msg[msg.size() - 1] == '\n')
            msg.erase(msg.size() - 1);
    }
    else
        cmd = line;

    for (int i = 0; i < 14; i++)
    {
        if (isCmd[i](cmd) == true)
        {
            switch (i)
            {
            case 0:
                capCmd(msg, Server, fd);
                break ;
            
            case 1:
                passCmd(msg, Server, fd);
                break ;

            case 2:
                nickCmd(msg, Server, fd);
                break ;

            case 3:
                userCmd(msg, Server, fd);
                break ;
            
            case 4:
                pingCmd(msg, fd);
                break ;

            case 5:
                pongCmd(msg, Server, fd);
                break ;
                
            case 6:
                quitCmd(msg, Server, fd);
                break ;

            case 7:
                joinCmd(msg, Server, fd);
                break ;

            case 8:
                topicCmd(msg, Server, fd);
                break ;

            case 9:
                privmsgCmd(msg, Server, fd);
                break ;

            case 10:
                modeCmd(msg, Server, fd);
                break ;

            case 11:
                inviteCmd(msg, Server, fd);
                break ;

            case 12:
                partCmd(msg, Server, fd);
                break ;

            case 13:
                kickCmd(msg, Server, fd);
                break ;
            }
        }
    }
}
