#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"

class Channel
{
	private:
		bool inviteOnly;
		int limit;
		bool topic_restriction;
		std::string name;
		std::string created_at;
		std::string time_creation;
		std::string password;
		std::string topic_name;
		std::vector<Client*> clients;
		std::vector<Client*> admins;

		/* ******************** Setters ******************** */
		void set_createiontime(void);

	public:
		Channel();
		~Channel();
		Channel(Channel const &src);
		Channel &operator=(Channel const &src);

		/* ******************** Getters ******************** */
		bool GetInvitOnly(void);
		int GetLimit(void);
		int GetClientsNumber(void);
		bool GetTopicRestriction(void) const;
		bool clientInChannel(std::string &nick);
		std::string GetTopicName(void);
		std::string GetPassword(void);
		std::string GetName(void);
		std::string GetTime(void);
		std::string get_creationtime(void);
		std::string clientChannel_list(void);
		Client *get_client(int fd);
		Client *get_client(std::string nick);
		Client *get_admin(int fd);
		Client *GetClientInChannel(std::string name);

		/* ******************** Setters ******************** */
		void SetInvitOnly(bool inviteOnly);
		void SetLimit(int limit);
		void SetTopicName(std::string topic_name);
		void SetPassword(std::string password);
		void SetName(std::string name);
		void SetTime(std::string time);
		void setTopicRestriction(bool value);

		/* ******************** Methods ******************** */
		void add_client(Client *newClient);
		void add_admin(Client *newClient);
		void rmClient(int fd);
		void rmAdmin(int fd);
		std::string GetClientList(void);

		/* ******************** SenToAll ******************** */
		void sendToAll(std::string rpl1);
		void sendToAll_but_not_him(std::string rpl1, int fd);
};

#endif