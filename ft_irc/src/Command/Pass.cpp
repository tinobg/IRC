#include "Server.hpp"

void	Server::pass_cmd(int fd, std::string cmd)
{
	cmd = cmd.substr(4);
	Client *cli = GetClient(fd);
	size_t pos = cmd.find_first_not_of("\t\v ");
	if (pos == std::string::npos)
		{_sendResponse(ERR_NEEDMOREPARAMS(cli->GetNickName()), fd); return;}
	cmd = cmd.substr(pos);
	if (!cli->getRegistered())
	{
		if (cmd == password)
			cli->setRegistered(true);
		else
			_sendResponse(ERR_INCORPASS(cli->GetNickName()), fd);
	}
	else
		_sendResponse(ERR_ALREADYREGISTERED(cli->GetNickName()), fd);
}
