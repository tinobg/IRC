#include "Server.hpp"

void	Server::list_cmd(int fd, std::string cmd)
{
	cmd = cmd.substr(4);
	std::string badParams;
	std::stringstream ss(cmd);
	ss >> badParams;
	Client	*cli = GetClient(fd);
	if (!badParams.empty())
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd));
	_sendResponse(RPL_LISTSTART(cli->GetNickName()), fd);
	std::vector<Channel*> cList = this->GetAllChans();
	for (std::vector<Channel*>::iterator it = cList.begin(); it != cList.end(); ++it)
	{
		std::stringstream ss;
		ss << (*it)->GetClientsNumber();
		_sendResponse(RPL_LIST((*it)->GetName(), ss.str(), (*it)->GetTopicName()), fd);
	}
	_sendResponse(RPL_LISTEND(cli->GetNickName()), fd);
}
