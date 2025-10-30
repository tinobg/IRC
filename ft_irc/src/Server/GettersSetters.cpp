#include "Server.hpp"

/* ******************** Getters ******************** */
int Server::GetPort(void) { return (this->port); }

int Server::GetFd(void) { return (this->server_fdsocket); }

std::string Server::GetPassword(void) { return (this->password); }

Client *Server::GetClient(int fd)
{
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i]->GetFd() == fd)
			return (this->clients[i]);
	}
	return (NULL);
}

Client *Server::GetClient(std::string nickname)
{
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i]->GetNickName() == nickname)
			return (this->clients[i]);
	}
	return (NULL);
}

size_t	Server::GetChanID(std::string chanName)
{
	size_t	i = 0;
	while (i < this->channels.size())
	{
		if (this->channels[i]->GetName() == chanName)
			return i;
		i++;
	}
	return INT_MAX;
}

Channel	*Server::GetChan(std::string chanName)
{
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		if (this->channels[i]->GetName() == chanName)
			return (this->channels[i]);
	}
	return NULL;
}

std::vector<Channel*>	Server::GetAllChans(void) { return this->channels;}

std::string	Server::getDrawPath(size_t i)
{
	if (i >= this->draw.size())
		ft_error("getDrawPath bad parameeters given");
	return (this->draw[i]);
}

/* ******************** Setters ******************** */
void Server::SetFd(int fd) { this->server_fdsocket = fd; }

void Server::SetPort(int port) { this->port = port; }

void Server::SetPassword(std::string password) { this->password = password; }

void Server::AddClient(Client *newClient) { this->clients.push_back(newClient); }

void Server::AddChannel(Channel *newChannel) { this->channels.push_back(newChannel); }

void Server::AddFds(pollfd newFd) { this->fds.push_back(newFd); }

void Server::set_sever_socket()
{
	int en = 1;
	add.sin_family = AF_INET;
	add.sin_addr.s_addr = INADDR_ANY;
	add.sin_port = htons(port);
	server_fdsocket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fdsocket == -1)
		ft_error("failed to create socket");
	if(setsockopt(server_fdsocket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		ft_error("failed to set option (SO_REUSEADDR) on socket");
	if (fcntl(server_fdsocket, F_SETFL, O_NONBLOCK) == -1)
		ft_error("failed to set option (O_NONBLOCK) on socket");
	if (bind(server_fdsocket, (struct sockaddr *)&add, sizeof(add)) == -1)
		ft_error("failed to bind socket");
	if (listen(server_fdsocket, SOMAXCONN) == -1)
		ft_error("listen() failed");
	new_cli.fd = server_fdsocket;
	new_cli.events = POLLIN;
	new_cli.revents = 0;
	fds.push_back(new_cli);
}
