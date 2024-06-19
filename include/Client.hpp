#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ft_irc.hpp"

class Client
{
private:
    std::string    _cap_ls;
    std::string    _pass;
    std::string    _nick;
    std::string    _user;
private:
    Client(Client const & src);
    //Client& operator=(Client const & rhs);

public:
    Client(void) {};
    ~Client(void) {};

    std::string const & getCapLs(void) const {return this->_cap_ls;};
    std::string const & getPass(void) const {return this->_pass;};
    std::string const & getNick(void) const {return this->_nick;};
    std::string const & getUser(void) const {return this->_user;};

    void                setCapLs(std::string const & cap_ls) {this->_cap_ls = cap_ls;};
    void                setPass(std::string const & pass) {this->_pass = pass;};
    void                setNick(std::string const & nick) {this->_nick = nick;};
    void                setUser(std::string const & user) {this->_user = user;};
};

#endif