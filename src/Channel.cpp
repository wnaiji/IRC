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

void    Channel::setTopic(std::string const & topic)
{
    this->_topic = topic;
    return ;
}