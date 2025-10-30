#ifndef UTILS_HPP
# define UTILS_HPP

# include "AllDef.hpp"

bool	isPortValid(std::string port);
bool	isPasswordValid(std::string pw);
void	*getServ(void *p);
void	ft_error(std::string msg);

/* ******************** Send Methods ******************** */
void	senderror(int code, std::string clientname, int fd, std::string msg);
void	senderror(int code, std::string clientname, std::string channelname, int fd, std::string msg);
void	_sendResponse(std::string response, int fd);

#endif
