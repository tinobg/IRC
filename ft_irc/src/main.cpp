#include "Server.hpp"

int main(int ac, char** av)
{
	if (ac != 3)
	{
		std::cout << "Usage: " << av[0] << " <port number> <password>" << std::endl;
		return 1;
	}
	Server *server = new Server();
	if (!server)
		ft_error("new Server() failled!!!");		
	getServ(server);
	std::cout << "---- SERVER ----" << std::endl;
	try
	{
		signal(SIGINT, Server::SignalHandler);
		signal(SIGQUIT, Server::SignalHandler);
		signal(SIGPIPE, SIG_IGN); // or MSG_NOSIGNAL flag in send() to ignore SIGPIPE on linux systems
		if(!isPortValid(av[1]) || !isPasswordValid(av[2]))
			ft_error("invalid Port number / Password!");
		server->startServer(std::atoi(av[1]), av[2]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	delete server;
	std::cout << "The Server Closed!" << std::endl;
	return 0;
}
