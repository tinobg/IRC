#include "Server.hpp"

std::vector<std::string> Server::splitByLine(std::string str)
{
	std::vector<std::string> vec;
	std::istringstream stm(str);
	std::string line;
	while (std::getline(stm, line))
	{
		size_t found = line.find_first_of("\r");
		if (found != std::string::npos)
			line.erase(found, 1);
		vec.push_back(line);
	}
	return (vec);
}

std::vector<std::string> Server::splitBySpace(std::string &cmd)
{
	std::vector<std::string> vec;
	std::istringstream stm(cmd);
	std::string token;
	while (stm >> token)
	{
		vec.push_back(token);
		token.clear();
	}
	return (vec);
}

int	get_cmd_type(std::string &cmd, bool registered)
{
	std::istringstream stm(cmd);
	std::string token;
	stm >> token;
	for (size_t i = 0; i < token.size(); i++)
		token[i] = std::tolower(token[i]);
	if (token == "pass")
		return (PASS);
	if (token == "nick")
		return (NICK);
	if (token == "user")
		return (USER);
	if (!registered)
		return (NOTREGISTERED);
	if (token == "quit")
		return (QUIT);
	if (token == "kick")
		return (KICK);
	if (token == "join")
		return (JOIN);
	if (token == "topic")
		return (TOPIC);
	if (token == "mode")
		return (MODE);
	if (token == "part")
		return (PART);
	if (token == "invite")
		return (INVITE);
	if (token == "privmsg")
		return (PRIVMSG);
	if (token == "list")
		return (LIST);
	if (token == "bot")
		return (BOT);
	return (CMDNOTFOUND);
}

bool	isRegistered(int fd)
{
	Server	*serv = (Server*)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client	*cli = serv->GetClient(fd);
	if (!cli || cli->GetNickName().empty() \
	|| cli->GetUserName().empty() || !cli->GetLogedIn())
		return false;
	return true;
}

void	Server::parse_exec_cmd(std::string &cmd, int fd)
{
	size_t found = cmd.find_first_not_of(" \t\v");
	if(found != std::string::npos)
		cmd = cmd.substr(found);
	else
		return ;
	int cmd_type = get_cmd_type(cmd, isRegistered(fd));
	if (cmd_type == CMDNOTFOUND)
		_sendResponse(ERR_CMDNOTFOUND(GetClient(fd)->GetNickName(), splitBySpace(cmd)[0]), fd);
	else if (cmd_type == NOTREGISTERED)
		_sendResponse(ERR_NOTREGISTERED(std::string("")), fd);
	else
	{
		if (cmd.size() > MAX_CMD_LENGTH)
			{_sendResponse(ERR_MAXCMDLENGTH(cmd.substr(0, MAX_CHAR_TRONC) + "..."), fd); return;}
		void (Server::*ptr_cmd[])(int, std::string) = 
		{
			&Server::pass_cmd,
			&Server::nick_cmd,
			&Server::user_cmd,
			&Server::quit_cmd,
			&Server::kick_cmd,
			&Server::join_cmd,
			&Server::topic_cmd,
			&Server::mode_cmd,
			&Server::part_cmd,
			&Server::invite_cmd,
			&Server::privmsg_cmd,
			&Server::list_cmd,
			&Server::bot_cmd
		};
		(this->*ptr_cmd[cmd_type])(fd, cmd);
	}
}
