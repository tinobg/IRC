#ifndef SERVER_HPP
# define SERVER_HPP

# include "Utils.hpp"
# include "Channel.hpp"

class Server
{
	private:
		int port;
		int server_fdsocket;
		static bool Signal;
		std::string password;
		std::vector<Client*> clients;
		std::vector<Channel*> channels;
		std::vector<struct pollfd> fds;
		struct sockaddr_in add;
		struct sockaddr_in cliadd;
		struct pollfd new_cli;
		std::vector<std::string> draw;

	public:
		Server();
		~Server();
		Server(Server const &src);
		Server &operator=(Server const &src);

		/* ******************** Getters ******************** */
		int	GetFd(void);
		int	GetPort(void);
		std::string GetPassword(void);
		Client	*GetClient(int fd);
		Client	*GetClient(std::string nickname);
		size_t	GetChanID(std::string chanName);
		Channel	*GetChan(std::string chanName);
		std::vector<Channel*>	GetAllChans(void);
		std::string	getDrawPath(size_t i);

		/* ******************** Setters ******************** */
		void	SetFd(int server_fdsocket);
		void	SetPort(int port);
		void	SetPassword(std::string password);
		void	AddClient(Client *newClient);
		void	AddChannel(Channel *newChannel);
		void	AddFds(pollfd newFd);
		void	set_sever_socket(void);

		/* ******************** Remove Methods ******************** */
		void	RemoveClient(int fd, std::string reason);
		void	RemoveChan(std::string chanName);

		/* ******************** Signal Methods ******************** */
		static void	SignalHandler(int signum);

		/* ******************** Server Methods ******************** */
		void	startServer(int port, std::string pass);
		void	accept_new_client(void);
		void	reciveNewData(int fd);

		/* ******************** Parsing Methods ******************** */
		static std::vector<std::string>	splitByLine(std::string str);
		static std::vector<std::string>	splitBySpace(std::string &str);
		void	parse_exec_cmd(std::string &cmd, int fd);

		/* ******************** Command Methods ******************** */
		void	pass_cmd(int fd, std::string cmd);
		void	nick_cmd(int fd, std::string cmd);
		void	user_cmd(int fd, std::string cmd);
		void	privmsg_cmd(int fd, std::string cmd);
		void	invite_cmd(int fd, std::string cmd);
		void	kick_cmd(int fd, std::string cmd);
		void	part_cmd(int fd, std::string cmd);
		void	quit_cmd(int fd, std::string cmd);
		void	list_cmd(int fd, std::string cmd);
		void	topic_cmd(int fd, std::string cmd);
		void	mode_cmd(int fd, std::string cmd);
		void	join_cmd(int fd, std::string cmd);
		void	bot_cmd(int fd, std::string cmd);
};

#endif