#include "Server.hpp"

bool	isValidNickAndChan(int fd, std::string nickName, std::string &chanName)
{
	Server	*serv = (Server*)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client	*cli = serv->GetClient(fd);
	Client	*cli2 = serv->GetClient(nickName);
	if (nickName.empty() || chanName.empty() || chanName[0] != '#')
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	if (!cli2)
		return (_sendResponse(ERR_NOSUCHNICK(cli->GetNickName()), fd), false);
	chanName.erase(0, 1);
	if (serv->GetChanID(chanName) == INT_MAX)
		return (_sendResponse(ERR_NOSUCHCHANNEL(cli->GetNickName(), chanName), fd), false);
	Channel* chan = serv->GetChan(chanName);
	if (!chan->get_client(fd))
		return (_sendResponse(ERR_NOTONCHANNEL(cli->GetNickName(), chanName), fd), false);
	if (chan->GetInvitOnly() && !chan->get_admin(fd))
		return (_sendResponse(ERR_CHANOPRIVSNEEDED(cli->GetNickName(), chanName), fd), false);
	if (chan->GetClientInChannel(nickName))
		return (_sendResponse(ERR_USERONCHANNEL(nickName, chanName), fd), false);
	return (!cli2->GetInviteChannel(chanName));
}

void	Server::invite_cmd(int fd, std::string cmd)
{
	cmd = cmd.substr(6);
	std::string nickName, chanName, badparam;
	std::stringstream ss(cmd);
	ss >> nickName;
	ss >> chanName;
	ss >> badparam;
	Client	*cli = GetClient(fd);
	if (!badparam.empty())
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd));
	if (!isValidNickAndChan(fd, nickName, chanName))
		return ;
	Client	*cli2 = GetClient(nickName);
	cli2->AddChannelInvite(chanName);
	_sendResponse(RPL_INVITING(nickName, chanName), fd);
	_sendResponse(RPL_INVITED(cli->GetNickName(), chanName), cli2->GetFd());
}
