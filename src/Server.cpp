#include "Server.hpp"


Server::Server(int const & pPort, string const & pPassword) 
: _port(pPort), _password(pPassword), _pingMsg("42serv_4428")
{
    return ;
}

void    Server::init(void)
{   
    this->_addr.sin_family = AF_INET;
    this->_addr.sin_port = htons(this->_port);
    this->_addr.sin_addr.s_addr = INADDR_ANY;

    this->_fd_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_fd_socket == -1)
        throw std::runtime_error("Error: Server::init: socket()");
    if (fcntl(this->_fd_socket, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Error: Server::init: fcntl()");
    const int opt_on = 1;
    if (setsockopt(this->_fd_socket, SOL_SOCKET, SO_REUSEADDR, &opt_on, sizeof(opt_on)) == -1)
        throw std::runtime_error("Error: Server::init: setsockopt()");
    if (bind(this->_fd_socket, reinterpret_cast<t_sockaddr *>(&this->_addr), sizeof(this->_addr)) == -1)
        throw std::runtime_error("Error: Server::init: bind()");     
    if (listen(this->_fd_socket, SOMAXCONN) == -1)
        throw std::runtime_error("Error: Server::init: listen()");

    this->_fd_epoll = epoll_create1(0);
    if (this->_fd_epoll == -1)
        throw std::runtime_error("Error: Server::init: epoll_create1()");
    t_epoll_event   event;
    event.events = EPOLLIN;
    event.data.fd = this->_fd_socket;
    if (epoll_ctl(this->_fd_epoll, EPOLL_CTL_ADD, this->_fd_socket, &event) == -1)
        throw std::runtime_error("Error: Server::init: epoll_ctl()");
}

static void putClient(int const & fd, std::string const & nick, std::string const & user)
{
    std::cout << std::endl;
    COUT_CYAN("[FD:     " << fd << "]");
    COUT_CYAN("[NICK:   " << nick << "]");
    COUT_CYAN("[USER:   " << user << "]");
    std::cout << std::endl;
}

void    Server::run(void)
{
    int             event_count;
    t_epoll_event   events[MAX_EVENTS];
    int             client_fd;
    char            read_buffer[READ_BUFFER_SIZE + 1];
    
    memset(read_buffer, 0, sizeof(read_buffer));
    while (true)
    {
        COUT_GREEN("[START]");
        event_count = epoll_wait(this->_fd_epoll, events, MAX_EVENTS, -1);

        for (int i = 0; i < event_count; i++)
        {
            if (events[i].data.fd == this->_fd_socket)
            {
                struct sockaddr_in 			clientAddr;
                socklen_t   addr_len = sizeof(clientAddr);
                client_fd = accept(this->_fd_socket, (struct sockaddr *)&clientAddr, &addr_len);
                if (client_fd == -1 || client_fd == 0)
                    COUT_RED("Error: Server::run: accept(): " << strerror(errno));
                else
                {
                    COUT_BLUE("[NEW CONNECTION]");
                    std::cout << std::endl;
                    this->_clients[client_fd].init(clientAddr, client_fd);
                    struct epoll_event event = this->_clients[client_fd].getEvent();
                    if (epoll_ctl(this->_fd_epoll, EPOLL_CTL_ADD, client_fd, &event) == -1)
                        COUT_RED("Error: Server::run: epoll_ctl()" << strerror(errno));
                    break ;
                }
            }
            else
            {
                ssize_t bytes_read;
                bytes_read = recv(events[i].data.fd, read_buffer, READ_BUFFER_SIZE, 0);
                if (bytes_read <= 0)
                {
                    COUT_RED("Error: Server::run: recv(): " << strerror(errno));
                    this->_clients.erase(events[i].data.fd);
                }
                else if (bytes_read == READ_BUFFER_SIZE)
                    COUT_RED("<client> :Input line was too long");
                else if (bytes_read != 0)
                {
                    read_buffer[bytes_read] = '\0';
                    string msg(read_buffer);
                    if (msg.find('\n') != std::string::npos)
                    {
                        putClient(events[i].data.fd, this->_clients[events[i].data.fd].getNick(), this->_clients[events[i].data.fd].getUser());
                        if (this->_clients[events[i].data.fd]._history.empty() == false)
                            msg = this->_clients[events[i].data.fd]._history += msg;
                        this->_clients[events[i].data.fd].newMsg(msg, *this, events[i].data.fd);
                    }
                    else
                        this->_clients[events[i].data.fd]._history += msg;
                }
            }
        }
    }
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

string const &  Server::getPingMsg(void) const
{
    return this->_pingMsg;
}

void    Server::setPassword(string const & pNewPassword)
{
    this->_password = pNewPassword;
}

