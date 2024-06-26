#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ft_irc.hpp"

class Server;

class Client
{
private:
    bool                _isNew;
    bool                _cap;
    bool                _pass;
    std::string         _nick;
    std::string         _user;
    struct epoll_event  _event;
    int                 _client_fd;
    bool                _waitingForPong;

private:
    void                        initClient(std::string const & line);

public:
    Client(void);
    ~Client(void);

    struct sockaddr_in 		_addr;

    bool const &                getCap(void) const;
    bool const &                getPass(void) const;
    std::string const &         getNick(void) const;
    std::string const &         getUser(void) const;
    bool const &                getIsNew(void) const;
    int const &                 getFd(void) const;
    struct epoll_event const &  getEvent(void) const;
    bool const &                getWaitingForPong(void) const;

    void                        setCap(void);
    void                        setPass(void);
    void                        setNick(std::string const & nick);
    void                        setUser(std::string const & user);
    void                        setIsNew(bool const & status);
    void                        setFd(int const & fd);
    void                        setEvent(void);
    void                        setWaitingForPong(bool const & status);

    void                        init(struct sockaddr_in const & addr, int const & fd);
    void                        newMsg(std::string const & msg, Server & server, int const & fd);
};

#endif