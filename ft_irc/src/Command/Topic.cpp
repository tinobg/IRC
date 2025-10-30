#include "Server.hpp"

bool	isGoodParam(int fd, std::string &chanName, std::string &topic, bool &flag)
{
	Server	*serv = (Server*)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client	*cli = serv->GetClient(fd);
	if (chanName.empty() || chanName[0] != '#')
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	if (serv->GetChanID(chanName.substr(1)) == INT_MAX)
		return (_sendResponse(ERR_NOSUCHCHANNEL(cli->GetNickName(), chanName), fd), false);
	chanName = chanName.substr(1);
	Channel	*chan = serv->GetChan(chanName);
	if (!chan->get_client(fd))
		return (_sendResponse(ERR_NOTONCHANNEL(cli->GetNickName(), chanName), fd), false);
	if (topic.empty())
		return (true);
	if (topic[0] != ':')
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	topic = topic.substr(1);
	if (chan->GetTopicRestriction() && !chan->get_admin(fd))
		return (_sendResponse(ERR_CHANOPRIVSNEEDED(cli->GetNickName(), chanName), fd), false);
	return (flag = true, true);
}

void	Server::topic_cmd(int fd, std::string cmd)
{
	cmd.erase(0, 5);
	std::stringstream ss(cmd);
	std::string chanName, topic;
	ss >> chanName;
	Client	*cli = this->GetClient(fd);
	while (!ss.eof())
	{
		std::string tmp;
		ss >> tmp;
		topic += tmp + " ";
	}
	if (!topic.empty())
		topic.erase(topic.size() - 1);
	bool	flag = false;
	if (!isGoodParam(fd, chanName, topic, flag))
		return ;
	Channel	*chan = this->GetChan(chanName);
	if (flag)
	{
		chan->SetTopicName(topic);
		chan->sendToAll(RPL_TOPICWHOTIME(cli->GetNickName(), chanName, chan->GetTopicName(), chan->GetTime()));
	}
	else
	{
		if (chan->GetTopicName().empty())
			_sendResponse(RPL_NOTOPIC(cli->GetNickName(), chanName), fd);
		else
			_sendResponse(RPL_TOPIC(cli->GetNickName(), chanName, chan->GetTopicName()), fd);
	}
}
