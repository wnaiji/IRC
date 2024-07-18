#ifndef SENDMSG_HPP
# define SENDMSG_HPP

# include "ft_irc.hpp"
# include <ctime>

class Server;
class Channel;

class SendMsg
{
public:
    static void PING(std::string const & pMsg, int const & fd);
    static void CAP(int const & index, int const & fd);
    static void QUIT(std::string const & cmd, int const & fd);
    static void JOINS(std::string const & name, Server & Server, int const & fd);
    static void PART(std::string const & name, Server & Server, int const & fd);

    static void RPL_WELCOME(Server & Server, int const & fd);
    static void RPL_YOURHOST(Server & Server, int const & fd);
    static void RPL_CREATED(Server & Server, int const & fd);
    static void RPL_MYINFO(Server & Server, int const fd);
    static void RPL_TOPIC(Channel const & Channel, Server & Server, int const & fd);
    static void RPL_NAMREPLY(Channel const & Channel, Server & Server, int const & fd);
    static void RPL_ENDOFNAMES(Channel const & Channel, Server & Server, int const & fd);
    static void RPL_NOTOPIC(Channel const & Channel, Server & Server, int const & fd);
    static void RPL_CHANNELMODEIS(Channel const & Channel, Server & Server, int const & fd);
    static void RPL_INVITING(std::string const & channel, std::string const & nick, Server & Server, int const & fd);

    static void ERR_PASSWDMISMATCH(int const & fd);
    static void ERR_NICKNAMEINUSE(std::string const & nick, int const & fd);
    static void ERR_NONICKNAMEGIVEN(int const & fd);
    static void ERR_ERRONEUSNICKNAME(std::string const & nick, int const & fd);
    static void ERR_NEEDMOREPARAMS(std::string const & cmd, int const & fd);
    static void ERR_ALREADYREGISTERED(int const & fd);
    static void ERR_CANNOTSENDTOCHAN(std::string const & nameChan, Server & Server, int const & fd);
    static void ERR_NOSUCHNICK(std::string const & client, Server & Server, int const & fd);
    static void ERR_NOSUCHCHANNEL(std::string const & channel, Server & Server, int const & fd);
    static void ERR_NOTONCHANNEL(std::string const & channel, Server & Server, int const & fd);
    static void ERR_CHANOPRIVSNEEDED(std::string const & channel, Server & Server, int const & fd);
    static void ERR_CHANNELISFULL(std::string const & channel, Server & Server, int const & fd);
    static void ERR_BADCHANNELKEY(std::string const & channel, Server & Server, int const & fd);
    static void ERR_INVITEONLYCHAN(std::string const & channel, Server & Server, int const & fd);
    static void ERR_USERONCHANNEL(std::string const & channel, std::string const & nick, Server & Server, int const & fd);
};

#endif