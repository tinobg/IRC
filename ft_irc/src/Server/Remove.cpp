#include "Server.hpp"

void	RmThisFdFromAllChans(int fd, std::string reason)
{
	Server	*serv = (Server*)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client *cli = serv->GetClient(fd);
	std::vector<Channel*> AllChan = serv->GetAllChans();
	for (std::vector<Channel*>::iterator it = AllChan.begin(); it != AllChan.end(); it++)
	{
		if ((*it)->get_client(fd))
		{
			(*it)->rmClient(fd);
			(*it)->sendToAll(RPL_QUIT(cli->GetNickName(), (*it)->GetName(), reason));
			if ((*it)->GetClientsNumber() == 0)
				serv->RemoveChan((*it)->GetName());
		}
	}
}

void	Server::RemoveClient(int fd, std::string reason)
{
	size_t i = 0;
	while (i < this->fds.size())
	{
		if (this->fds[i].fd == fd)
			{this->fds.erase(this->fds.begin() + i);break;}
		i++;
	}
	for (i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i]->GetFd() == fd)
		{
			RmThisFdFromAllChans(fd, reason);
			delete this->clients[i];
			this->clients.erase(this->clients.begin() + i);
			break;
		}
	}
}

void	Server::RemoveChan(std::string chanName)
{
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		if (this->channels[i]->GetName() == chanName)
		{
			delete this->channels[i];
			this->channels.erase(this->channels.begin() + i);
			break;
		}
	}
}
