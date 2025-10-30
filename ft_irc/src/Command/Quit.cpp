#include "Server.hpp"

bool	isGoodParams(int fd, std::string reason)
{
	Server *serv = (Server*)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client *cli = serv->GetClient(fd);
	if (!reason.empty())
	{
		for (size_t i = 0; i < reason.size(); i++)
		{
			if (!isalpha(reason[i]) || reason[i] != ' ')
				return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
		}
	}
	return (true);
}

void	Server::quit_cmd(int fd, std::string cmd)
{
	std::string reason = cmd.substr(4);
	while (!reason.empty() && isspace(reason[0]))
		reason = reason.substr(1);
	if (!isGoodParams(fd, reason))
		return ;
	this->RemoveClient(fd, reason);
}
