#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ft_irc.hpp"

class Client
{
private:
    bool                _isNew;
    std::string         _pass;
    std::string         _nick;
    std::string         _user;
    struct epoll_event  _event;
    int                 _client_fd;

private:
    //Client(Client const & src);

public:
    Client(void);
    ~Client(void);

    std::string const &         getPass(void) const;
    std::string const &         getNick(void) const;
    std::string const &         getUser(void) const;
    bool const &                getIsNew(void) const;
    int const &                 getFd(void) const;
    struct epoll_event const &  getEvent(void) const;

    void                        setPass(std::string const & pass);
    void                        setNick(std::string const & nick);
    void                        setUser(std::string const & user);
    void                        setIsNew(bool const & status);
    void                        setFd(int const & fd);
    void                        setEvent(void);
};

#endif