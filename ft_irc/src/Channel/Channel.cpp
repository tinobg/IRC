#include "Channel.hpp"

Channel::Channel()
{
	this->inviteOnly = false;
	this->limit = MAX_MODE_L;
	this->topic_restriction = false;
	this->name = "";
	this->topic_name = "";
}

Channel::~Channel() {}

Channel::Channel(Channel const &src) { *this = src; }

Channel &Channel::operator=(Channel const &src)
{
	if (this != &src)
	{
		this->inviteOnly = src.inviteOnly;
		this->limit = src.limit;
		this->topic_restriction = src.topic_restriction;
		this->name = src.name;
		this->password = src.password;
		this->topic_name = src.topic_name;
		this->clients = src.clients;
		this->admins = src.admins;
	}
	return *this;
}

/* ******************** Methods ******************** */
void Channel::add_client(Client *newClient)
{
	clients.push_back(newClient);
	if (clients.size() == 1)
		add_admin(newClient);
}

void Channel::add_admin(Client *newClient) { admins.push_back(newClient); }

void Channel::rmAdmin(int fd)
{
	for (std::vector<Client*>::iterator it = admins.begin(); it != admins.end(); ++it)
	{
		if ((*it)->GetFd() == fd)
		{
			admins.erase(it);
			break;
		}
	}
}

void Channel::rmClient(int fd)
{
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->GetFd() == fd)
		{
			clients.erase(it);
			rmAdmin(fd);
			break;
		}
	}
}

std::string Channel::GetClientList(void)
{
	std::string clientslist = "";
	for (size_t i = 0; i < clients.size(); i++)
		clientslist += BN + clients[i]->GetNickName();
	return clientslist;
}
