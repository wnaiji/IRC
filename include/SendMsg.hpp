#ifndef SENDMSG_HPP
# define SENDMSG_HPP

# include "ft_irc.hpp"

class Server;

class SendMsg
{
public:
    //creation des methodes pour le reponse de commande 
    static void RPL_WELCOME(Server & Server, int const & fd);
    static void RPL_YOURHOST(Server & Server, int const & fd);
    static void RPL_CREATED(Server & Server, int const & fd);

    static void ERR_PASSWDMISMATCH(int const & fd);
    static void ERR_NICKNAMEINUSE(std::string const & nick, int const & fd);
    static void ERR_NONICKNAMEGIVEN(int const & fd);
    static void ERR_ERRONEUSNICKNAME(std::string const & nick, int const & fd);
    static void ERR_NEEDMOREPARAMS(std::string const & cmd, int const & fd);
    static void ERR_ALREADYREGISTERED(int const & fd);
};

#endif