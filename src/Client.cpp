#include "Client.hpp"

/*---------------CONSTRUCTOR---------------*/

Client::Client(void)
{
    return ;
}

Client::~Client(void)
{
    close(this->_client_fd);
    return ;
}

/*---------------GETTER---------------*/

std::string const & Client::getPass(void) const
{
    return this->_pass;
}

std::string const & Client::getNick(void) const
{
    return this->_nick;
}

std::string const & Client::getUser(void) const
{
    return this->_user;
}

bool const &    Client::getIsNew(void) const
{
    return this->_isNew;
}

int const & Client::getFd(void) const
{
    return this->_client_fd;
}

struct epoll_event const & Client::getEvent(void) const
{
    return this->_event;
}

/*---------------SETTER---------------*/

void                Client::setPass(std::string const & pass)
{
    this->_pass = pass;
    return ;
}

void                Client::setNick(std::string const & nick)
{
    if (this->_isNew == false)
        this->_nick = nick;
    return ;
}

void                Client::setUser(std::string const & user)
{
    if (this->_isNew == false)
        this->_user = user;
    return ;
}

void                Client::setIsNew(bool const & status)
{
    this->_isNew = status;
    return ;
}

void                Client::setFd(int const & fd)
{
    this->_client_fd = fd;
    fcntl(this->_client_fd, F_SETFL, O_NONBLOCK);
    return ;
}

void                Client::setEvent(void)
{
    this->_event.events = EPOLLIN | EPOLLET;
    this->_event.data.fd = this->_client_fd;
    return ;
}

/*--------------MANAGEMENT_CLIENTS--------------*/


