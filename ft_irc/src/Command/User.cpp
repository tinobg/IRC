#include "Server.hpp"

bool	onlyAlphanum(std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!std::isalnum(str[i]))
			return false;
	}
	return true;
}

bool	isArealName(std::string &str)
{
	size_t	i = 0;
	while (!str.empty() && isspace(str[0]))
		str.erase(0, 1);
	if (str[0] != ':')
		return false;
	str.erase(0, 1);
	if (str.empty())
		return false;
	while (i < str.size())
	{
		if (!std::isalnum(str[i]) && !std::isspace(str[i]))
			return false;
		i++;
	}
	return true;
}

bool	isGoodParams(int fd, std::string userName, std::string hostName, std::string serverName, std::string &realName)
{
	Server	*serv = (Server*)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client *cli = serv->GetClient(fd);
	if (userName.empty() || realName.empty() || hostName.empty() || serverName.empty())
		return (_sendResponse(ERR_NEEDMOREPARAMS(cli->GetNickName()), fd), false);
	if (!onlyAlphanum(userName))
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	if(!cli->getRegistered())
		return (_sendResponse(ERR_NOTREGISTERED(std::string("")), fd), false);
	if (!isArealName(realName))
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	if (!cli->GetUserName().empty())
		return (_sendResponse(ERR_ALREADYREGISTERED(cli->GetNickName()), fd), false);
	return true;
}

void	Server::user_cmd(int fd, std::string cmd)
{
	cmd.erase(0, 4);
	std::string userName, hostName, serverName, realName;
	std::stringstream ss(cmd);
	ss >> userName >> hostName >> serverName;
	Client *cli = GetClient(fd);
	getline(ss, realName);
	if (!isGoodParams(fd, userName, hostName, serverName, realName))
		return ;
	cli->SetUsername(realName);
	cli->setLogedin(true);
	_sendResponse(RPL_CONNECTED(cli->GetNickName()), fd);
}
