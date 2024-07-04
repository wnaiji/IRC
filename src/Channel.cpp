#include "Channel.hpp"

Channel::Channel(void)
{
    return ;
}

Channel::Channel(std::string const & name) : _name(name)
{
    return ;
}

Channel::~Channel(void)
{
    return ;
}

std::string const & Channel::getName(void) const
{
    return this->_name;
}

std::string const & Channel::getTopic(void) const
{
    return this->_topic;
}

std::string const & Channel::getNameAdmin(void) const
{
    return this->_nameAdmin;
}

void    Channel::setTopic(std::string const & topic)
{
    this->_topic = topic;
    return ;
}

void    Channel::setNameAdmin(std::string const & nameAdmin)
{
    this->_nameAdmin = nameAdmin;
    return ;
}
