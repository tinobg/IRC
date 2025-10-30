#include "Server.hpp"

std::vector<std::string> getStrList(std::string strList)
{
	std::vector<std::string> strList_vec;
	std::stringstream ss(strList);
	std::string str;
	while (std::getline(ss, str, ','))
		strList_vec.push_back(str);
	return (strList_vec);
}

bool	isValidChan(int fd, std::string chanName, std::string chanPw)
{
	Server	*serv = (Server *)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client	*cli = serv->GetClient(fd);
	Channel* chan = NULL;
	if (serv->GetChanID(chanName) == INT_MAX)
	{
		if (chanName.empty() || chanName.size() > MAX_CHAN_NAME)
			return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
		for (size_t i = 0; i < chanPw.size(); i++)
		{
			if (!isalnum(chanPw[i]))
				return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
		}
		chan = new Channel();
		if (!chan)
			ft_error("new Channel failed");
		chan->SetName(chanName);
		chan->SetPassword(chanPw);
		if (serv->GetAllChans().size() == MAX_CHAN_NB)
			return (_sendResponse(ERR_SERVERFULL_CHAN(cli->GetNickName()), fd), false);
		serv->AddChannel(chan);
	}
	else
	{
		chan = serv->GetChan(chanName);
		if (chan->GetInvitOnly())
		{
			if (!cli->GetInviteChannel(chanName))
				return (_sendResponse(ERR_INVITEONLYCHAN(cli->GetNickName(), chanName), fd), false);
		}
		if (chan->GetClientsNumber() == chan->GetLimit())
			return (_sendResponse(ERR_CHANNELISFULL(cli->GetNickName(), chanName), fd), false);
		if (!chan->GetPassword().empty() && chan->GetPassword() != chanPw)
			return (_sendResponse(ERR_INCORPASS(cli->GetNickName()), fd), false);
		if (chan->get_client(fd))
			return (_sendResponse(ERR_USERONCHANNEL(cli->GetNickName(), chanName), fd), false);
	}
	return (true);
}

bool	isGoodParams(int fd, std::string chanList, std::string chanPw, std::map<std::string, std::string> &chanName_pw)
{
	Server	*serv = (Server *)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client	*cli = serv->GetClient(fd);
	std::vector<std::string> chanList_vec = getStrList(chanList);
	std::vector<std::string> chanPw_vec = getStrList(chanPw);
	if (chanList_vec.empty())
		return (_sendResponse(ERR_NOSUCHCHANNEL(cli->GetNickName(), chanList), fd), false);
	if (chanList_vec.size() > MAX_JOIN_CHAN_AT_ONCE)
		return (_sendResponse(ERR_TOOMANYCHANNELS(cli->GetNickName()), fd), false);
	for (size_t i = 0; i < chanList_vec.size(); i++)
	{
		if (chanList_vec[i][0] != '#')
			return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	}
	for (size_t i = 0; i < chanList_vec.size(); i++)
	{
		std::string pw = "";
		if (chanPw_vec.size() > i)
			pw = chanPw_vec[i];
		chanName_pw.insert(std::make_pair(chanList_vec[i].substr(1), pw));
	}
	for (std::map<std::string, std::string>::iterator it = chanName_pw.begin(); it != chanName_pw.end(); it++)
	{
		if (!isValidChan(fd, it->first, it->second))
			it->second = SKIP;
	}
	return (!chanName_pw.empty());
}

void	Server::join_cmd(int fd, std::string cmd)
{
	cmd = cmd.substr(4);
	std::string chanList, chanPw, badparam;
	std::stringstream ss(cmd);
	ss >> chanList >> chanPw >> badparam;
	Client	*cli = GetClient(fd);
	if (!badparam.empty())
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd));
	std::map<std::string, std::string> chanName_pw;
	if (!isGoodParams(fd, chanList, chanPw, chanName_pw))
		return ;
	for (std::map<std::string, std::string>::iterator it = chanName_pw.begin(); it != chanName_pw.end(); it++)
	{
		if (it->second == SKIP)
			continue;
		Channel *chan = this->GetChan(it->first);
		chan->add_client(cli);
		cli->RmChannelInvite(it->first);
		chan->sendToAll(RPL_JOIN(cli->GetNickName(), it->first));
		if (!chan->GetTopicName().empty())
			_sendResponse(RPL_TOPIC(cli->GetNickName(), it->first, chan->GetTopicName()), fd);
		_sendResponse(RPL_NAMREPLY(cli->GetNickName(), it->first, chan->GetClientList()), fd);
		_sendResponse(RPL_ENDOFNAMES(cli->GetNickName(), it->first), fd);
	}
}
