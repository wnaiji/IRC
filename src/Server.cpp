#include "Server.hpp"

void		Server::manageCmd(std::string const & pMsg, int client_fd)
{
    (void) pMsg;
    (void) client_fd;
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
    {
        close(this->_fd_epoll);
        throw std::runtime_error("Error: Server::init: epoll_ctl()");        
    }
    //this->startPingLoop();
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
        cout << "polling start" << endl;
        event_count = epoll_wait(this->_fd_epoll, events, MAX_EVENTS, -1);
        cout << "events ready : " << event_count << endl;
        for (int i = 0; i < event_count; i++)
        {
            if (events[i].data.fd == this->_fd_socket)
            {
                struct sockaddr_in 			clientAddr;
                socklen_t   addr_len = sizeof(clientAddr);
                client_fd = accept(this->_fd_socket, (struct sockaddr *)&clientAddr, &addr_len);
                if (client_fd == -1 || client_fd == 0)
                    cerr << "Error: Server::run: accept(): " << strerror(errno) << std::endl;
                else
                {
                    cout << "new connection client_fd: " << client_fd << endl;
                    this->_clients[client_fd].init(clientAddr, client_fd);
                    struct epoll_event event = this->_clients[client_fd].getEvent();
                    if (epoll_ctl(this->_fd_epoll, EPOLL_CTL_ADD, client_fd, &event) == -1)
                        cerr << "Error: Server::run: epoll_ctl()" << endl;
                }
            }
            else
            {
                cout << "reading fd : " << client_fd << endl;
                ssize_t bytes_read;
                bytes_read = recv(events[i].data.fd, read_buffer, READ_BUFFER_SIZE, 0);
                if (bytes_read <= 0)
                    this->_clients.erase(events[i].data.fd);
                else if (bytes_read == READ_BUFFER_SIZE)
                    cerr << "<client> :Input line was too long" << endl; //ERR_INPUTTOOLONG
                else if (bytes_read != 0)
                {
                    read_buffer[bytes_read] = '\0';
                    string msg(read_buffer);

                    this->_clients[events[i].data.fd].newMsg(msg, *this, events[i].data.fd);
                    cout << "=====> " << events[i].data.fd << endl;
                    cout << "=====> " << this->_clients[events[i].data.fd].getNick() << endl;
                    cout << "=====> " << this->_clients[events[i].data.fd].getUser() << endl;
                }
                else
                    cout << "bytes_read == 0 to see " << bytes_read << endl;
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

void    Server::sendPing(void)
{
    std::map<int, Client>::iterator it;
    std::string                     msg = "42serv_4428";

    this->_pingMsg = msg;
    for (it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        SendMsg::PING(msg, it->second.getFd());
        it->second.setWaitingForPong(false);
    }
    sleep(60);
    if (it == this->_clients.end())
        return ;
    for (it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if (it->second.getWaitingForPong() == false)
            this->_clients.erase(it->second.getFd());
    }
}

void*   timeoutCheckerThread(void* arg) {
    Server* server = static_cast<Server*>(arg);
    while (true) {
        sleep(60);
        server->sendPing();
    }
    return NULL;
}

void    Server::startPingLoop(void)
{
    pthread_t thread;
    pthread_create(&thread, NULL, timeoutCheckerThread, this);
    pthread_detach(thread);
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
