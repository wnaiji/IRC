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
# include <vector>
# include <cerrno> // for strerror and global errno
# include <cstring>
# include <pthread.h>
# include <csignal>
# include <sys/wait.h>
// end to see
# include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event

//fe
# include <sstream>

# include "Client.hpp"
# include "Channel.hpp"

#define MAX_EVENTS 5000
#define READ_BUFFER_SIZE 512

using std::string;
using std::cout; // to rm ?
using std::cerr;
using std::endl;

typedef struct sockaddr_in	t_sockaddr_in;
typedef struct in_addr		t_in_addr;
typedef struct sockaddr		t_sockaddr;
typedef struct epoll_event	t_epoll_event;

class Client;
class Channel;

class Server
{
	private:
		const int				_port;
		std::string				_password;
		int						_fd_socket;
		t_sockaddr_in 			_addr;
		int						_fd_epoll;
		pthread_t 				_thread;
		std::string const		_pingMsg;
	private:
		Server(void);
		Server(Server const & pSrc);
	public:
		Server(int const & pPort, string const & pPassword);
		~Server(void);
		
		std::map<int, Client>			_clients;
		std::map<std::string, Channel>	_channels;

		void			init(void);
		void			run(void);
		//void			sendPing(void);
		//void			startPingLoop(void);
		//void    		disck(void);

		int const &		getPort(void) const;
		string const &	getPassword(void) const;
		string const &	getPingMsg(void) const;


		static void*   	timeoutCheckerThread(void* arg);
		void			setPassword(string const & pNewPassword);
};

#endif
