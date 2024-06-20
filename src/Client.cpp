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

    /*reception de la str entier    *
    * pars pour recup la 1er ligne  *
    * recup par la suite du 1er mot *
    * check si commande valide      *
    * continuer si tout est bon ou  *
    * demande du mot de pass sinon  *
    * disconnect le new client      */

static bool checkPass(std::string const & line, std::string const & pass)
{
    int         start = 0;
    std::string tmp;

    for (int i = 0; line[i]; i++)
    {
        if (line[i] == ' ')
        {
            tmp = line.substr(start, i);
            start = i + 1;
            break ;
        }
    }
    if (tmp == "PASS")
        tmp = line.substr(start, line.size());
    cout << "###=>" << tmp << "<=###" << pass << endl;
    return tmp == pass;
}

void    Client::initClient(std::string const & line)
{
    int         start = 0;
    std::string tmp;

    for (int i = 0; line[i]; i++)
    {
        if (line[i] == ' ')
        {
            tmp = line.substr(start, i);
            start = i + 1;
            break ;
        }
    }
    if (tmp == "NICK")
    {
        tmp = line.substr(start, line.size());
        this->setNick(tmp);
    }
    else if (tmp == "USER" && !this->_nick.empty())
    {
        tmp = line.substr(start, line.size());
        this->setUser(tmp);
    }
    return ;
}

void    Client::newClient(std::string const & msg, std::string const & pass)
{
    int     start = 0;
    string  tmp;

    for (int i = 0; msg[i]; i++)
    {
        if (msg[i] == '\n' || msg[i] == '\r')
        {
            tmp = msg.substr(start, i);
            if (checkPass(tmp, pass) == true)
                this->setIsNew(false);
            else if (this->_isNew == false)
                initClient(tmp);
            start = i + 1;
        }
    }
    if (tmp.empty() || this->getIsNew() == false)
    {
        tmp = msg.substr(start, msg.size());
        initClient(tmp);
    }
}
