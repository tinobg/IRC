#include "Server.hpp"

bool Server::Signal = false;

Server::Server()
{
	this->port = 0;
	this->server_fdsocket = -1;
	this->password = "";
}

Server::~Server()
{
	for(size_t i = 0; i < clients.size(); i++)
		delete clients[i];
	for(size_t i = 0; i < channels.size(); i++)
		delete channels[i];
	if (server_fdsocket != -1)
	{
		std::cout << RED << "Server <" << server_fdsocket << "> Disconnected" << WHI << std::endl;
		close(server_fdsocket);
	}
}

Server::Server(Server const &src)
{
	*this = src;
}

Server &Server::operator=(Server const &src)
{
	if (this != &src){
		this->port = src.port;
		this->server_fdsocket = src.server_fdsocket;
		this->password = src.password;
		this->clients = src.clients;
		this->channels = src.channels;
		this->fds = src.fds;
		this->draw = src.draw;
	}
	return *this;
}

/* ******************** Signal Methods ******************** */
void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::Signal = true;
}

/* ******************** Server Methods ******************** */
void Server::startServer(int port, std::string pass)
{
	this->password = pass;
	this->port = port;
	this->set_sever_socket();

	std::ifstream file (".bonus");
	if (file.good())
	{
		file.close();
		file.open(JOKES_PATH);
		if (!file.is_open())
			ft_error("canot open file .jokes");
		file.close();
		file.open(CAT_PATH);
		if (!file.is_open())
			ft_error("canot open file .cat");
		file.close();
		file.open(CAR_PATH);
		if (!file.is_open())
			ft_error("canot open file .car");
		file.close();
		file.open(FACE_PATH);
		if (!file.is_open())
			ft_error("canot open file .face");
		file.close();
		file.open(RABBIT_PATH);
		if (!file.is_open())
			ft_error("canot open file .rabbit");
		file.close();
		file.open(BIKE_PATH);
		if (!file.is_open())
			ft_error("canot open file .bike");
		this->draw.push_back(CAT_PATH);
		this->draw.push_back(CAR_PATH);
		this->draw.push_back(RABBIT_PATH);
		this->draw.push_back(FACE_PATH);
		this->draw.push_back(BIKE_PATH);
	}

	std::cout << GRE << "Server <" << server_fdsocket << "> Connected" << WHI << std::endl;
	std::cout << "Waiting to accept a connection...\n";
	while (Server::Signal == false)
	{
		if((poll(&fds[0],fds.size(),-1) == -1) && Server::Signal == false)
			ft_error("poll() failed");
		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == server_fdsocket)
					this->accept_new_client();
				else
					this->reciveNewData(fds[i].fd);
			}
		}
	}
}

void Server::accept_new_client()
{
	memset(&cliadd, 0, sizeof(cliadd));
	socklen_t len = sizeof(cliadd);
	int incofd = accept(server_fdsocket, (sockaddr *)&(cliadd), &len);
	if (incofd == -1)
		{std::cout << "accept() failed" << std::endl; return;}
	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1)
		{std::cout << "fcntl() failed" << std::endl; return;}
	new_cli.fd = incofd;
	new_cli.events = POLLIN;
	new_cli.revents = 0;
	Client *cli = new Client();
	if (!cli)
	{
		std::cerr << "new Client() failled!!!" << std::endl;
		close(incofd);
		delete this;
		exit (EXIT_FAILURE);
	}
	cli->SetFd(incofd);
	cli->setIpAdd(inet_ntoa((cliadd.sin_addr)));
	clients.push_back(cli);
	fds.push_back(new_cli);
	std::cout << GRE << "Client <" << incofd << "> Connected" << WHI << std::endl;
}

void Server::reciveNewData(int fd)
{
	std::vector<std::string> cmd;
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	Client *cli = GetClient(fd);
	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0);
	if(bytes <= 0)
		RemoveClient(fd, SP);
	else
	{
		cli->setBuffer(buff);
		if(cli->getBuffer().find_first_of("\n") == std::string::npos)
			return;
		cmd = splitByLine(cli->getBuffer());
		for(size_t i = 0; i < cmd.size(); i++)
			parse_exec_cmd(cmd[i], fd);
		if(GetClient(fd))
			GetClient(fd)->setBuffer("");
	}
}
