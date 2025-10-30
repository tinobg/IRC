#include "Server.hpp"

std::vector<std::string> getStrL(std::string strList)
{
	std::vector<std::string> strList_vec;
	std::stringstream ss(strList);
	std::string str;
	while (std::getline(ss, str, ','))
		strList_vec.push_back(str);
	return (strList_vec);
}

bool	checkChanName(int fd, std::string &chanName)
{
	Server	*serv = (Server*)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client	*cli = serv->GetClient(fd);
	if (chanName.empty() || chanName[0] != '#')
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	chanName.erase(0, 1);
	if (serv->GetChanID(chanName) == INT_MAX)
		return (_sendResponse(ERR_NOSUCHCHANNEL(cli->GetNickName(), chanName), fd), false);
	Channel	*chan = serv->GetChan(chanName);
	if (!chan->GetClientInChannel(cli->GetNickName()))
		return (_sendResponse(ERR_NOTONCHANNEL(cli->GetNickName(), chanName), fd), false);
	if (!chan->get_admin(fd))
		return (_sendResponse(ERR_CHANOPRIVSNEEDED(cli->GetNickName(), chanName), fd), false);
	return (true);
}

bool	checkKickList(int fd, std::string &chanName, std::vector<std::string> &kStrList, std::vector<Client*> &kList)
{
	Server	*serv = (Server*)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client	*cli = serv->GetClient(fd);
	Channel	*chan = serv->GetChan(chanName);
	if (kStrList.empty())
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	if (kStrList.size() > MAX_KICK_LIST)
		return (_sendResponse(ERR_TOOMANYTARGETS(cli->GetNickName()), fd), false);
	for (std::vector<std::string>::iterator it = kStrList.begin(); it != kStrList.end(); ++it)
	{
		if (serv->GetClient(*it))
		{
			if (chan->GetClientInChannel(*it))
				kList.push_back(serv->GetClient(*it));
			else
				_sendResponse(ERR_NOTONCHANNEL((*it), chanName), fd);
		}
		else
			_sendResponse(ERR_NOSUCHNICK(*it), fd);
	}
	return (true);
}

bool	checkReason(int fd, std::string &reason)
{
	Server	*serv = (Server*)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client	*cli = serv->GetClient(fd);
	if (!reason.empty())
	{
		if (reason[0] != ':')
			return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
		reason.erase(0, 1);
		for (size_t i = 0; i < reason.size(); i++)
			if (!std::isalnum(reason[i]) && !std::isspace(reason[i]))
				return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	}
	return(true);
}

void	Server::kick_cmd(int fd, std::string cmd)
{
	cmd = cmd.substr(4);
	std::string chanName, kickList, reason, tmp;
	std::vector<Client*> kList;
	std::vector<std::string> kStrList;
	std::stringstream ss(cmd);
	ss >> chanName;
	ss >> kickList;
	while (!ss.eof())
	{
		ss >> tmp;
		reason += tmp + " ";
		tmp.clear();
	}
	if (!reason.empty())
		reason.erase(reason.size() - 1);
	kStrList = getStrL(kickList);
	if (!checkChanName(fd, chanName) || !checkKickList(fd, chanName, kStrList, kList) || !checkReason(fd, reason))
		return ;
	Client	*cli = GetClient(fd);
	Channel	*chan = this->GetChan(chanName);
	for (std::vector<Client*>::iterator it = kList.begin(); it != kList.end(); ++it)
	{
		chan->sendToAll(RPL_KICK(cli->GetNickName(), chanName, (*it)->GetNickName(), reason));
		chan->rmClient((*it)->GetFd());
	}
}
