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

void    manageCmd(std::string const & line, Server & Server, int const & fd)
{
    size_t      pos = line.find(' ');
    std::string cmd;
    std::string msg;
    bool        (*isCmd[])(std::string const &) = {isCap, isPass, isNick, isUser, isPing, \
                                                    isPong};

    if (pos != std::string::npos)
    {
        cmd = line.substr(0, pos);
        msg = line.substr(pos + 1, std::string::npos);
        if (msg[msg.size() - 1] == '\r' || msg[msg.size() - 1] == '\n')
            msg.erase(msg.size() - 1);
    }
    else
        cmd = line;

    for (int i = 0; i < 6; i++)
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
                pingCmd(line, fd);
                break ;

            case 5:
                pongCmd(line, Server, fd);
                break ;
            }
        }
    }
}
