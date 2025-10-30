#include "Utils.hpp"

bool	isPortValid(std::string port)
{
	int	nb;

	if (port.empty())
		return (false);
	for (size_t i = 0; i < port.size(); i++)
	{
		if (!isdigit(port[i]))
			return (false);
	}
	if (port.size() > 5 || port.size() < 4)
		return (false);
	nb = std::atoi(port.c_str());
	if (nb < PORT_AUTORISAT_MIN || nb > PORT_AUTORISAT_MAX)
		return (false);
	return (true);
}

bool	isPasswordValid(std::string pw)
{
	if (pw.empty() || pw.size() > 10)
		return (false);
	for (size_t i = 0; i < pw.size(); i++)
	{
		if (!isalnum(pw[i]))
			return (false);
	}
	return (true);
}

void	ft_error(std::string msg)
{
	throw(std::runtime_error(msg));
}

void	*getServ(void *p)
{
	static void *ser = NULL;
	if (!ser)
		ser = p;
	return ser;
}
