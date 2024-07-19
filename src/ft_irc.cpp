#include "ft_irc.hpp"

static int get_port(char const * port_str)
{
    char    *end;
    long int port = strtol(port_str, &end, 10); // to see ?
    if (*end != '\0')
    {
        COUT_RED("Error: get_port: parsing port");
        return(GET_PORT_ERROR_PARS);
    }
    else if (errno == ERANGE)
    {
        COUT_RED("Error: get_port: ERANGE port");
        return (GET_PORT_ERROR_LIMIT);
    }
    else if (port < PORT_MIN || port > PORT_MAX)
    {
        COUT_RED("Error: get_port: need to be in the range [" << PORT_MIN << ", " << PORT_MAX << "]");
        return (GET_PORT_ERROR_RANGE);
    }
    return (static_cast<int>(port));
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        COUT_RED("Error: main: args need to be in format './ircserv <port> <password>'");
        return 1;
    }
    int port = get_port(argv[1]);
    if (port <= GET_PORT_ERROR_RANGE)
        return (port);
    Server  server(port, argv[2]);
    try
    {
        server.init();
    }
    catch (std::exception const & e)
    {
        cerr << e.what() << endl;
        return 1;
    }
    signal(SIGINT, Server::SignalHandler);
    server.run();
}