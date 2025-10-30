#include "Client.hpp"

/* ******************** Getters ******************** */
int Client::GetFd() { return this->fd; }

bool Client::getRegistered() { return registered; }

bool Client::GetInviteChannel(std::string &ChName)
{
	for (size_t i = 0; i < this->ChannelsInvite.size(); i++)
	{
		if (this->ChannelsInvite[i] == ChName)
			return true;
	}
	return false;
}

std::string Client::GetNickName() { return this->nickname; }

bool Client::GetLogedIn() { return this->logedin; }

std::string Client::GetUserName() { return this->username; }

std::string Client::getBuffer() { return buffer; }

std::string Client::getIpAdd() { return ipadd; }

std::string Client::getHostname()
{
	std::string hostname = this->GetNickName() + "!" + this->GetUserName();
	return hostname;
}

/* ******************** Setters ******************** */
void Client::SetFd(int fd) { this->fd = fd; }

void Client::SetNickname(std::string &nickName) { this->nickname = nickName; }

void Client::setLogedin(bool value) { this->logedin = value; }

void Client::SetUsername(std::string &username) { this->username = username; }

void Client::setBuffer(std::string recived)
{
	if (!recived.empty())
		buffer += recived;
	else
		buffer.clear();
}

void Client::setRegistered(bool value) { registered = value; }

void Client::setIpAdd(std::string ipadd) { this->ipadd = ipadd; }
