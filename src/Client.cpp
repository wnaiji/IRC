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

bool const &    Client::getCap(void) const
{
    return this->_cap;
}

bool const &    Client::getPass(void) const
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

void                Client::setCap(void)
{
    this->_cap = true;
    return ;
}

void                Client::setPass(void)
{
    this->_pass = true;
    return ;
}

void                Client::setNick(std::string const & nick)
{
        this->_nick = nick;
    return ;
}

void                Client::setUser(std::string const & user)
{
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

void    Client::init(sockaddr_in6 const & addr, int const & fd)
{
    this->_client_fd = fd;
    this->setEvent();
    this->setIsNew(true);
    this->_addr = addr;
}

    /*reception de la str entier    *
    * pars pour recup la 1er ligne  *
    * recup par la suite du 1er mot *
    * check si commande valide      *
    * continuer si tout est bon ou  *
    * demande du mot de pass sinon  *
    * disconnect le new client      */

void    Client::newMsg(std::string const & msg, Server & Server, int const & fd)
{
    std::vector<std::string>    lines;
    std::istringstream          stream(msg);
    std::string                 line;

    while (std::getline(stream, line))
        lines.push_back(line);

    for (unsigned long i = 0; i < lines.size(); i++)
        manageCmd(lines[i], Server, fd);
}
