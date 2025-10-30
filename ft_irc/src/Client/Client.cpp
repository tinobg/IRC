#include "Client.hpp"

Client::Client()
{
	this->nickname = "";
	this->username = "";
	this->fd = -1;
	this->registered = false;
	this->buffer = "";
	this->ipadd = "";
	this->logedin = false;
}

Client::Client(std::string nickname, std::string username, int fd) : fd(fd), nickname(nickname), username(username)
{
}

Client::~Client()
{
	close(this->fd);
	std::cout << RED << "Client <" << this->fd << "> Disconnected" << WHI << std::endl;
}

Client::Client(Client const &src)
{
	*this = src;
}

Client &Client::operator=(Client const &src)
{
	if (this != &src)
	{
		this->nickname = src.nickname;
		this->username = src.username;
		this->fd = src.fd;
		this->ChannelsInvite = src.ChannelsInvite;
		this->buffer = src.buffer;
		this->registered = src.registered;
		this->ipadd = src.ipadd;
		this->logedin = src.logedin;
	}
	return *this;
}

/* ******************** Methods ******************** */
void Client::AddChannelInvite(std::string &chname)
{
	ChannelsInvite.push_back(chname);
}

void Client::RmChannelInvite(const std::string &chname)
{
	for (size_t i = 0; i < this->ChannelsInvite.size(); i++)
	{
		if (this->ChannelsInvite[i] == chname)
		{
			this->ChannelsInvite.erase(this->ChannelsInvite.begin() + i);
			return;
		}
	}
}
