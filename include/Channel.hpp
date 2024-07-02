#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include "Client.hpp"

class Client;

class Channel
{
private:
    std::string _name;
    std::string _topic;


public:
    Channel(void);
    Channel(std::string const & name);
    ~Channel(void);

    std::map<int, Client *>   _clients;
    
    std::string const & getName(void) const;
    std::string const & getTopic(void) const;

    void                setTopic(std::string const & topic);
};


#endif
