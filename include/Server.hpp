#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <cstdlib> // for strtol(), exit()
// to see
# include <stdexcept> // for runtime_error()
# include <unistd.h> // for close(), read()
# include <sys/socket.h>
# include <netinet/in.h> 
# include <arpa/inet.h>
# include <fcntl.h>
# include <map>
# include <cerrno> // for strerror and global errno
# include <cstring>
// end to see
# include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event

//fe
# include <sstream>

# include "ft_irc_error.hpp"
# include "Client.hpp"

#define MAX_EVENTS 5000
#define READ_BUFFER_SIZE 512

using std::string;
using std::cout; // to rm ?
using std::cerr;
using std::endl;

typedef struct sockaddr_in6	t_sockaddr_in6;
typedef struct in6_addr		t_in6_addr;
typedef struct sockaddr		t_sockaddr;
typedef struct epoll_event	t_epoll_event;

class Client;

class Server
{
	private:
		const int				_port;
		std::string				_password;
		int						_fd_socket;
		t_sockaddr_in6 			_addr;
		int						_fd_epoll;
		std::map<int, Client *>	_clients;
	private:
		Server(void);
		Server(Server const & pSrc);
		Server &	operator=(Server const & pRhs);
		void		manageCmd(std::string const & pMsg, std::map<int, Client *> & client, int client_fd); 
	public:
		Server(int const & pPort, string const & pPassword);
		~Server(void);

		void			init(void);
		void			run(void);

		int const &		getPort(void) const;
		string const &	getPassword(void) const;
		void			setPassword(string const & pNewPassword);
};

#endif
