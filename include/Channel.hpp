#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include "Client.hpp"

class Client;

class Channel
{
private:
    std::string                 _name;
    std::string                 _topic;
    std::vector<std::string>    _nameAdmin;
    std::vector<std::string>    _invitName;
    std::string                 _password;
    bool                        _inviteStatus;
    bool                        _topicStatus;
    bool                        _keyStatus;
    int                         _limit;

public:
    Channel(void);
    Channel(std::string const & name);
    ~Channel(void);

    std::map<int, Client *>   _clients;
    
    std::string const &                 getName(void) const;
    std::string const &                 getTopic(void) const;
    std::vector<std::string> const &    getNameAdmin(void) const;
    std::vector<std::string> const &    getInvitName(void) const;
    std::string const &                 getPassWord(void) const;
    bool const &                        getInviteStatus(void) const;
    bool const &                        getTopicStatus(void) const;
    bool const &                        getKeyStatus(void) const;
    int const &                         getLimit(void) const;

    void                                setTopic(std::string const & topic);
    void                                setNameAdmin(std::string const & cmd, std::string const & nameAdmin);
    void                                setInviteName(std::string const & cmd, std::string const & inviteName);
    void                                setPassWord(std::string const & passWord);
    void                                setInviteStatus(bool const & status);
    void                                setTopicStatus(bool const & status);
    void                                setKeyStatus(bool const & status);
    void                                setLimit(std::string const & Limit);

    void                                addMode(char const & mode, std::string const & arg, Server & server, int const & fd);
    void                                removeMode(char const & mode, std::string const & arg, Server & Server, int const & fd);
    void                                sendMsg(std::string & msg, std::string mode, std::string param);
};


#endif
