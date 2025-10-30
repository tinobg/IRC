#include "Server.hpp"

bool isGoodParams(int fd, std::string chanList, std::vector<Channel*> &cList, std::string &reason)
{
	Server *serv = (Server*)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client *cli = serv->GetClient(fd);
	if (!reason.empty())
	{
		if (reason[0] != ':')
			return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
		reason.erase(0, 1);
		for (size_t i = 0; i < reason.size(); i++)
		{
			if (!std::isalnum(reason[i]) && !std::isspace(reason[i]))
				return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
		}
	}
	if (chanList.empty())
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	std::string chanName;
	std::stringstream ss(chanList);
	while (std::getline(ss, chanName, ','))
	{
		if (chanName[0] != '#')
			return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
		if (serv->GetChanID(chanName.substr(1)) == INT_MAX)
			{_sendResponse(ERR_NOSUCHCHANNEL(cli->GetNickName(), chanName.substr(1)), fd); continue;};
		cList.push_back(serv->GetChan(chanName.substr(1)));
	}
	return (cList.size() > 0);
}

void	Server::part_cmd(int fd, std::string cmd)
{
	cmd = cmd.substr(4);
	std::string chanList, reason;
	std::vector<Channel*> cList;
	std::stringstream ss(cmd);
	ss >> chanList;
	while (!ss.eof())
	{
		std::string tmp;
		ss >> tmp;
		reason += tmp + " ";
	}
	if (!reason.empty())
		reason.erase(reason.size() - 1);
	if (!isGoodParams(fd, chanList, cList, reason))
		return ;
	Client	*cli = GetClient(fd);
	for (std::vector<Channel*>::iterator it = cList.begin(); it != cList.end(); ++it)
	{
		Channel *chan = *it;
		if (!chan->get_client(fd))
			{_sendResponse(ERR_NOTONCHANNEL(cli->GetNickName(), chan->GetName()), fd); continue;}
		chan->sendToAll(RPL_PART(cli->GetNickName(), chan->GetName(), reason));
		chan->rmClient(fd);
		if (!chan->GetClientsNumber())
			this->RemoveChan(chan->GetName());
	}
}
