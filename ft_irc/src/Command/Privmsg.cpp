#include "Server.hpp"

bool	isGoodParam(int fd, std::string &dest, std::string &msg)
{
	Server	*serv = (Server*)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client	*cli = serv->GetClient(fd);
	if (dest.empty())
		return (_sendResponse(ERR_NOSUCHNICK(std::string("")), fd), false);
	if (dest[0] == '#')
	{
		if (serv->GetChanID(dest.substr(1)) == INT_MAX)
			return (_sendResponse(ERR_NOSUCHCHANNEL(cli->GetNickName(), dest.substr(1)), fd), false);
		Channel	*chan = serv->GetChan(dest.substr(1));
		if ((chan->GetInvitOnly() || !chan->GetPassword().empty()) && !chan->get_client(fd))
			return (_sendResponse(ERR_CANNOTSENDTOCHAN(cli->GetNickName(), dest.substr(1)), fd), false);
	}
	else if (!serv->GetClient(dest))
		return (_sendResponse(ERR_NOSUCHNICK(dest), fd), false);
	while (!msg.empty() && std::isspace(msg[0]))
		msg.erase(0, 1);
	if (msg.empty())
		return (_sendResponse(ERR_NOTEXTTOSEND(cli->GetNickName()), fd), false);
	if (msg[0] != ':')
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	msg.erase(0, 1);
	while (!msg.empty() && std::isspace(msg[0]))
		msg.erase(0, 1);
	if (msg.empty())
		return (_sendResponse(ERR_NOTEXTTOSEND(cli->GetNickName()), fd), false);
	return true;
}

void	Server::privmsg_cmd(int fd, std::string cmd)
{
	cmd = cmd.substr(7);
	std::stringstream ss(cmd);
	std::string dest, msg, tmp;
	ss >> dest;
	while (ss.good())
	{
		ss >> tmp;
		msg += tmp + " ";
		tmp.clear();
	}
	Client	*cli = GetClient(fd);
	if (msg.empty())
		return (_sendResponse(ERR_NOTEXTTOSEND(cli->GetNickName()), fd));;
	msg.resize(msg.size() - 1);
	if (!isGoodParam(fd, dest, msg))
		return ;
	if (dest[0] != '#')
		return (_sendResponse(RPL_PRIVMSG(cli->GetNickName(), dest, msg), this->GetClient(dest)->GetFd()));
	else
	{
		Channel	*chan = GetChan(dest.substr(1));
		chan->sendToAll_but_not_him(RPL_PRIVMSG(cli->GetNickName(), dest, msg), fd);
	}
}
