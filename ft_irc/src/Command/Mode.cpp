#include "Server.hpp"

bool	isValidChan(int fd, std::string &chanName)
{
	Server	*serv = (Server *)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client	*cli = serv->GetClient(fd);
	if (chanName.empty())
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	if (chanName[0] != '#')
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	chanName.erase(0, 1);
	if (serv->GetChanID(chanName) == INT_MAX)
		return (_sendResponse(ERR_NOSUCHCHANNEL(cli->GetNickName(), chanName), fd), false);
	Channel	*chan = serv->GetChan(chanName);
	if (!chan->get_client(fd))
		return (_sendResponse(ERR_NOTONCHANNEL(cli->GetNickName(), chanName), fd), false);
	if (!chan->get_admin(fd))
		return (_sendResponse(ERR_CHANOPRIVSNEEDED(cli->GetNickName(), chanName), fd), false);
	return (true);
}

bool	isValidMode(int fd, std::string &modeStr, std::string &param, Channel *chan)
{
	Server	*serv = (Server *)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client	*cli = serv->GetClient(fd);
	if (modeStr.empty() || modeStr.size() > 2)
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	if (modeStr[0] != '+' && modeStr[0] != '-')
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	if (modeStr[1] != 'i' && modeStr[1] != 't' && modeStr[1] != 'k' && modeStr[1] != 'o' && modeStr[1] != 'l')
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	if ((modeStr[1] == 'i' || modeStr[1] == 't') && !param.empty())
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	if (modeStr[1] == 'l')
	{
		if ((param.empty() && modeStr[0] == '+') || param.size() > 3)
			return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
		for (size_t i = 0; i < param.size(); i++)
		{
			if (!isdigit(param[i]))
				return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
		}
		if (param.size() && (atoi(param.c_str()) < chan->GetClientsNumber() || atoi(param.c_str()) > MAX_MODE_L))
			return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	}
	if (modeStr[1] == 'k')
	{
		if (param.empty() && modeStr[0] == '+')
			return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
		for (size_t i = 0; i < param.size(); i++)
		{
			if (!isalnum(param[i]))
				return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
		}
	}
	if (modeStr[1] == 'o')
	{
		if (param.empty())
			return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
		Client	*cli = serv->GetClient(param);
		if (!cli)
			return (_sendResponse(ERR_NOSUCHNICK(param), fd), false);
		if (!chan->get_client(cli->GetFd()))
			return (_sendResponse(ERR_NOTONCHANNEL(cli->GetNickName(), chan->GetName()), fd), false);
	}
	return (true);
}

bool	isGoodParam(int fd, std::string &chanName, std::string &modeStr, std::string &param)
{
	Server	*serv = (Server *)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	if (!isValidChan(fd, chanName))
		return (false);
	Channel	*chan = serv->GetChan(chanName);
	if (!isValidMode(fd, modeStr, param, chan))
		return (false);
	return (true);
}

int		giveMode(const std::string &modeStr)
{
	int mode;
	switch(modeStr[1])
	{
		case 'i': mode = MODE_I; break;
		case 't': mode = MODE_T; break;
		case 'k': mode = MODE_K; break;
		case 'o': mode = MODE_O; break;
		case 'l': mode = MODE_L; break;
		default: return -1;
	}
	return mode * 10 + (modeStr[0] == '+');
}

void	mod_i(Channel *chan, int flag, std::string param)
{
	(void)param;
	chan->SetInvitOnly(flag);
}

void	mod_t(Channel *chan, int flag, std::string param)
{
	(void)param;
	chan->setTopicRestriction(flag);
}


void	mod_k(Channel *chan, int flag, std::string param)
{
	if (flag)
		chan->SetPassword(param);
	else
		chan->SetPassword("");
}

void	mod_o(Channel *chan, int flag, std::string param)
{
	(void)param;
	Client	*cli = chan->get_client(param);
	if (flag)
	{
		if (!chan->get_admin(cli->GetFd()))
			chan->add_admin(cli);
	}
	else
		chan->rmAdmin(cli->GetFd());
}

void	mod_l(Channel *chan, int flag, std::string param)
{
	if (flag)
		chan->SetLimit(atoi(param.c_str()));
	else
		chan->SetLimit(MAX_MODE_L);
}

void	Server::mode_cmd(int fd, std::string cmd)
{
	cmd.erase(0, 4);
	std::string	chanName, modeStr, param, badParam;
	std::stringstream	ss(cmd);
	ss >> chanName >> modeStr >> param >> badParam;
	Client	*cli = GetClient(fd);
	if (!badParam.empty())
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd));
	if (!isGoodParam(fd, chanName, modeStr, param))
		return ;
	Channel	*chan = this->GetChan(chanName);
	int	mode = giveMode(modeStr);
	void (*ptr_mod[])(Channel *, int, std::string) = 
		{
			&mod_i,
			&mod_t,
			&mod_k,
			&mod_o,
			&mod_l,
		};
	ptr_mod[mode / 10](chan, mode % 2, param);
	chan->sendToAll(RPL_MODE(cli->GetNickName(), chan->GetName(), modeStr, param));
}
