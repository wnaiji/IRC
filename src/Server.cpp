#include "Server.hpp"

void		Server::manageCmd(std::string const & pMsg, std::map<int , Client *> & client, int client_fd)
{
    string  trimMsg = pMsg;
    
    if ((client.find(client_fd) == client.end() || !client[client_fd]))
    {
        client[client_fd] = new Client();
        client[client_fd]->setCapLs(trimMsg);
    }
    else if (client[client_fd]->getPass().empty())
    {
        trimMsg.erase(0, 5);
        client[client_fd]->setPass(trimMsg);
    }
    else if (client[client_fd]->getNick().empty())
    {
        trimMsg.erase(0, 5);
        client[client_fd]->setNick(trimMsg);
    }
    else if (client[client_fd]->getUser().empty())
    {
        trimMsg.erase(0, 5);
        client[client_fd]->setUser(trimMsg);
    }
}

Server::Server(int const & pPort, string const & pPassword) 
: _port(pPort), _password(pPassword)
{
}

void    Server::init(void)
{
    const t_in6_addr  set_sin6_addr = IN6ADDR_ANY_INIT;
    
    this->_addr.sin6_family = AF_INET6;
    this->_addr.sin6_port = htons(this->_port);
    this->_addr.sin6_flowinfo = 0;
    this->_addr.sin6_addr = set_sin6_addr;
    this->_addr.sin6_scope_id = 0;

    this->_fd_socket = socket(AF_INET6, SOCK_STREAM, 0);
    if (this->_fd_socket == -1)
        throw std::runtime_error("Error: Server::init: socket fail");

    if (fcntl(this->_fd_socket, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Error: Server::init: fcntl()");
    //const int opt_off = 0;
    //if (setsockopt(this->_fd_socket, IPPROTO_IPV6, IPV6_V6ONLY, &opt_off, sizeof(opt_off)) == -1)
    //    throw std::runtime_error("Error: Server::init: first setsockopt fail");
    const int opt_on = 1;
    if (setsockopt(this->_fd_socket, SOL_SOCKET, SO_REUSEADDR, &opt_on, sizeof(opt_on)) == -1)
        throw std::runtime_error("Error: Server::init: second setsockopt fail");


    if (bind(this->_fd_socket, reinterpret_cast<t_sockaddr *>(&this->_addr), sizeof(this->_addr)) == -1)
        throw std::runtime_error("Error: Server::init: bind fail");     
    if (listen(this->_fd_socket, SOMAXCONN) == -1)
        throw std::runtime_error("Error: Server::init: listen fail");

    this->_fd_epoll = epoll_create1(0);
    if (this->_fd_epoll == -1)
        throw std::runtime_error("Error: Server::init: epoll_create1 fail");
    t_epoll_event   event;
    event.events = EPOLLIN;
    event.data.fd = this->_fd_socket;
    if (epoll_ctl(this->_fd_epoll, EPOLL_CTL_ADD, this->_fd_socket, &event) == -1)
    {
        close(this->_fd_epoll);
        throw std::runtime_error("Error: Server::init: epoll_ctl this->_fd_socket fail");        
    }
}

void    Server::run(void)
{
    int             event_count;
    t_epoll_event   event;
    t_epoll_event   events[MAX_EVENTS];
    int             client_fd;
    char            read_buffer[READ_BUFFER_SIZE + 1];
    while (true)
    {
        cout << "polling start" << endl;
        event_count = epoll_wait(this->_fd_epoll, events, MAX_EVENTS, -1);
        cout << "events ready : " << event_count << endl;
        for (int i = 0; i < event_count; i++)
        {
            if (events[i].data.fd == this->_fd_socket)
            {
                socklen_t   addr_len = sizeof(this->_addr);
                client_fd = accept(this->_fd_socket, reinterpret_cast<t_sockaddr *>(&this->_addr), &addr_len);
                if (client_fd == -1)
                    cerr << "Error: Server::run: accept fail" << endl;
                else
                {
                    cout << "new connection" << endl;
                    event.events = EPOLLIN | EPOLLET;
                    event.data.fd = client_fd;
                    if (epoll_ctl(this->_fd_epoll, EPOLL_CTL_ADD, client_fd, &event) == -1)
                        cerr << "Error: Server::run: epoll_ctl client_fd fail" << endl;
                    this->_clients[client_fd] = NULL;
                }
            }
            else
            {
                cout << "reading fd : " << events[i].data.fd << endl;
                ssize_t bytes_read;
                bytes_read = recv(events[i].data.fd, read_buffer, READ_BUFFER_SIZE, 0);
                read_buffer[bytes_read] = '\0';
                if (bytes_read == -1)
                    cerr << "Error: Server::run: recv fail." << endl;
                else if (bytes_read == READ_BUFFER_SIZE)
                    cerr << "<client> :Input line was too long" << endl;
                else if (bytes_read != 0)
                {
                    string msg(read_buffer);
                    size_t last_find = 0;
                    for (size_t i = 0; i < msg.length(); i++)
                    {
                        if (msg[i] == '\r' || msg[i] == '\n')
                        {   
                            //gestion des commandes les une apres les autres
                            //gestion des retours d'erreur avec errno
                            //liste des commandes -> pars des cmds -> gestion en fonction des cmds
                            manageCmd(msg.substr(last_find, i - last_find), this->_clients, client_fd);
                            i++;
                            if (i < msg.length() && msg[i] == '\n')
                                i++;
                            last_find = i;
                        }
                    }
                    std::cout << "========> " << client_fd << _clients[client_fd]->getCapLs() << std::endl;
                    std::cout << "========> " << client_fd << _clients[client_fd]->getPass() << std::endl;
                    std::cout << "========> " << client_fd << _clients[client_fd]->getNick() << std::endl;
                    std::cout << "========> " << client_fd << _clients[client_fd]->getUser() << std::endl;
                    if (this->_clients[client_fd]->getPass() != this->_password)
                        cout << "Error: " << this->_clients[client_fd]->getNick() << ": MDP: faild." << endl;
                    if (last_find < msg.length())
                    {
                        //gestion des commandes des commandes ici
                        cout << "last cmd not end with pair of r n or r or n" << endl;
                        //manageCmd(msg.substr(last_find), client, client_fd);
                    }                    
                }
                else
                    cout << "bytes_read == 0 to see " << bytes_read << endl;

                cout << "read : " << endl << ">>'" << read_buffer << "'<<" << endl;
            }
        }
    }
    cout << "FIN" << endl;
    return ;
}

Server::~Server(void)
{
    close(this->_fd_epoll);
    close(this->_fd_socket);
    return ;
}

int const & Server::getPort(void) const
{
    return (this->_port);
}

string const &  Server::getPassword(void) const
{
    return (this->_password);
}

void    Server::setPassword(string const & pNewPassword)
{
    this->_password = pNewPassword;
}
