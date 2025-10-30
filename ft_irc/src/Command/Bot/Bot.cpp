#include "Server.hpp"

bool	ft_isAnOperation(char c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/' );
}

bool	ft_isAgoodArg(std::string const &str)
{
	int	nb_int = 0;
	int	nb_op = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] != ' ')
		{
			if (isdigit(str[i]))
				nb_int++;
			else if (ft_isAnOperation(str[i]))
			{
				nb_op++;
				if (nb_int < nb_op + 1)
					return false;
			}
			else
				return false;
		}
	}
	return (nb_int == nb_op + 1);
}

bool	isGoodParam(int fd, std::string type, std::string param)
{
	Server	*serv = (Server *)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client	*cli = serv->GetClient(fd);
	if (type.empty() ||(type != "RPN" && type != "joke" && type != "draw"))
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	if ((type == "joke" || type == "draw") && !param.empty())
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	if (type == "RPN")
	{
		if (param.empty() || !ft_isAgoodArg(param))
			return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd), false);
	}
	return (true);
}

void	drawIt(int fd)
{
	Server	*serv = (Server *)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	std::srand(std::time(0));
	int randomNumber = (std::rand() % 5);
	std::string filePath = serv->getDrawPath(randomNumber);
	std::ifstream file(filePath.c_str());
	if (!file.good())
		ft_error("open failed");
	std::string line;
	while (getline(file, line))
		{line = line + BN; _sendResponse(line, fd);}
	file.close();
}

void	joke(int fd)
{
	std::srand(std::time(0));
	int randomNumber = (std::rand() % 8);
	std::ifstream file(JOKES_PATH);
	if (!file.good())
		ft_error("open failed");
	std::string line;
	int i = 0;
	while (getline(file, line) && i != randomNumber)
		i++;
	line = line + BN;
	_sendResponse(line, fd);
	file.close();
}

void	RPN(int fd, std::string str)
{
	Server	*serv = (Server *)getServ(NULL);
	if (!serv)
		ft_error("getServ failed");
	Client	*cli = serv->GetClient(fd);
	std::stack<int>	myStack;
	for(size_t i = 0; i < str.length(); i++)
	{
		if (str[i] != ' ')
		{
			if (isdigit(str[i]))
				myStack.push(str[i] - '0');
			else
			{
				int tmp = myStack.top();
				myStack.pop();
				switch (str[i])
				{
					case '+':
						tmp += myStack.top();
						break;
					case '-':
						tmp = myStack.top() - tmp;
						break;
					case '*':
						tmp *= myStack.top();
						break;
					default:
						if (!tmp)
							return (_sendResponse(ERR_DIVZERO(cli->GetNickName()), fd));
						tmp = myStack.top() / tmp;
						break;
				}
				myStack.pop();
				myStack.push(tmp);
			}
		}
	}
	std::stringstream res;
	res << "result is " << myStack.top() << "\r\n";
	_sendResponse(res.str(), fd);
}

void	Server::bot_cmd(int fd, std::string cmd)
{
	Client	*cli = GetClient(fd);
	std::ifstream	file(".bonus");
	if (!file.good())
		return (_sendResponse(ERR_BADPARAM(cli->GetNickName()), fd));
	cmd.erase(0, 4);
	std::stringstream	ss(cmd);
	std::string	type, param;
	ss >> type;
	getline(ss, param);
	if (!isGoodParam(fd, type, param))
		return ;
	if (type == "joke")
		return joke(fd);
	else if (type == "RPN")
		return RPN(fd, param);
	return drawIt(fd);
}
