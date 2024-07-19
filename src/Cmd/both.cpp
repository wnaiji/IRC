#include "ft_irc.hpp"

void    putMsg(std::string const & pMsg, Server & Server, int const & fd)
{
    std::string msg = ":42serv NOTICE * :[" + Server._clients[fd].getNick() + "] " + pMsg + "\r\n";
    for (std::map<int, Client>::iterator it = Server._clients.begin(); it != Server._clients.end(); it++)
        send(it->first, msg.c_str(), msg.size(), 0);
    return ;
}

void    bothCmd(std::string const & pMsg, Server & Server, int const & fd)
{
    if (pMsg.empty() || pMsg == "INIT")
    {
        Server.initBoth();
        putMsg("The BOT has been initialized! Try to find the golden number between 1 and 100. Good luck!", Server, fd);
    }
    else if (Server.getBothStatus() == true)
    {
        std::istringstream iss(pMsg);
        int value;
        iss >> value;
        if (iss.fail())
            putMsg("Your guess is incorrect. Please enter a number between 1 and 100.", Server, fd);
        else if (value > Server.getBothValue())
            putMsg("Your guess is too high. Try a lower number.", Server, fd);
        else if (value < Server.getBothValue())
            putMsg("Your guess is too low. Try a higher number.", Server, fd);
        else if (value == Server.getBothValue())
        {
            putMsg("Congratulations! You have guessed the correct number!", Server, fd);
            Server.setBothStatus();
        }
    }
    else
        putMsg("Please initialize the bot first before making guesses.", Server, fd);
    return ;
}
