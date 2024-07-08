#include "Channel.hpp"

Channel::Channel(void)
{
    return ;
}

Channel::Channel(std::string const & name) : _name(name)
{
    this->_inviteStatus = false;
    this->_topicStatus = false;
    this->_keyStatus = false;
    return ;
}

Channel::~Channel(void)
{
    return ;
}

/*---------------GETTER---------------*/

std::string const & Channel::getName(void) const
{
    return this->_name;
}

std::string const & Channel::getTopic(void) const
{
    return this->_topic;
}

std::vector<std::string> const & Channel::getNameAdmin(void) const
{
    return this->_nameAdmin;
}

std::vector<std::string> const & Channel::getInvitName(void) const
{
    return this->_invitName;
}

std::string const & Channel::getPassWord(void) const
{
    return this->_password;
}

bool const &    Channel::getInviteStatus(void) const
{
    return this->_inviteStatus;
}

bool const &    Channel::getTopicStatus(void) const
{
    return this->_topicStatus;
}

bool const &    Channel::getKeyStatus(void) const
{
    return this->_keyStatus;
}

int const &     Channel::getLimit(void) const
{
    return this->_limit;
}

/*---------------SETTER---------------*/

void    Channel::setTopic(std::string const & topic)
{
    this->_topic = topic;
    return ;
}

void    Channel::setNameAdmin(std::string const & cmd, std::string const & nameAdmin)
{
    if (cmd == "+o")
    {
        for (std::vector<std::string>::iterator it = this->_nameAdmin.begin(); it != this->_nameAdmin.end(); it++)
        {
            if (*it == nameAdmin)
                return ;
        }
        this->_nameAdmin.push_back(nameAdmin);
    }
    else
    {
        for (std::vector<std::string>::iterator it = this->_nameAdmin.begin(); it != this->_nameAdmin.end(); it++)
        {
            if (*it == nameAdmin)
                this->_nameAdmin.erase(it);
        }
    }
    return ;
}

void    Channel::setInviteName(std::string const & cmd, std::string const & inviteName)
{
    if (cmd == "+i")
    {
        for (std::vector<std::string>::iterator it = this->_invitName.begin(); it != this->_invitName.end(); it++)
        {
            if (*it == inviteName)
                return ;
        }
        this->_invitName.push_back(inviteName);
    }
    else
    {
        for (std::vector<std::string>::iterator it = this->_invitName.begin(); it != this->_invitName.end(); it++)
        {
            if (*it == inviteName)
                this->_invitName.erase(it);
        }
    }
}

void    Channel::setPassWord(std::string const & passWord)
{
    this->_password = passWord;
    return ;
}

void    Channel::setInviteStatus(bool const & status)
{
    this->_inviteStatus = status;
    return ;
}

void    Channel::setTopicStatus(bool const & status)
{
    this->_topicStatus = status;
    return ;
}

void    Channel::setKeyStatus(bool const & status)
{
    this->_keyStatus = status;
    return ;
}

void    Channel::setLimit(std::string const & pLimit)
{
    std::istringstream  iss(pLimit);
    int                 limit;
    iss >> limit;
    
    if (iss.fail())
        return ;
    this->_limit = limit;
    return ;
}

void    Channel::addMode(char const & mode, std::string const & pArg, Server & Server, int const & fd)
{
    (void)Server;
    (void)fd;
    if (mode == 'i')
    {
        this->_inviteStatus = true;
        //send message
    }
    else if (mode == 't')
    {
        this->_topicStatus = true;
        //send message
    }
    else if (mode == 'k')
    {
        this->_keyStatus = true;
        this->setPassWord(pArg);
        //send message
    }
    else if (mode == 'o')
    {
        this->setNameAdmin("+o", pArg);
        //send message
    }
    else if (mode == 'l')
    {
        this->setLimit(pArg);
        //sendMessage
    }
}

void    Channel::removeMode(char const & mode, std::string const & pArg, Server & Server, int const & fd)
{
    (void)Server;
    (void)fd;
    if (mode == 'i')
    {
        this->_inviteStatus = false;
        this->_invitName.clear();
        //send message
    }
    else if (mode == 't')
    {
        this->_topicStatus = false;
        //send message
    }
    else if (mode == 'k')
    {
        this->_keyStatus = false;
        this->_password.clear();
        //send message
    }
    else if (mode == 'o')
    {
        this->setNameAdmin("-o", pArg);
        //send message
    }
    else if (mode == 'l')
    {
        this->_limit = 0;
        //sendMessage
    }
}
