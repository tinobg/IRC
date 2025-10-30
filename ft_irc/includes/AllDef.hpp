#ifndef ALLDEF_HPP
# define ALLDEF_HPP

# include <arpa/inet.h>
# include <cctype>
# include <csignal>
# include <cstdlib>
# include <cstring>
# include <ctime>
# include <climits>
# include <fcntl.h>
# include <fstream>
# include <iostream>
# include <netdb.h>
# include <netinet/in.h>
# include <poll.h>
# include <sstream>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h>
# include <vector>
# include <map>
# include <string>
# include <stack>

/* ******************** Colors Macro ******************** */
# define RED "\e[1;31m"
# define WHI "\e[0;37m"
# define GRE "\e[1;32m"
# define YEL "\e[1;33m"

/* ******************** Command Macro ******************** */
# define PASS 0
# define NICK 1
# define USER 2
# define QUIT 3
# define KICK 4
# define JOIN 5
# define TOPIC 6
# define MODE 7
# define PART 8
# define INVITE 9
# define PRIVMSG 10
# define LIST 11
# define BOT 12
# define CMDNOTFOUND 13
# define NOTREGISTERED 14

/* ******************** Mode Macro ******************** */
# define MODE_I 0
# define MODE_T 1
# define MODE_K 2
# define MODE_O 3
# define MODE_L 4

#define MAX_MODE_L 10

# define SKIP "-----SKIP THIS ONE-----"

#define RPA_MIN 65500 // random port autorisated min
#define RPA_MAX 65535 // random port autorisated max

# define PORT_AUTORISAT_MIN 1024
# define PORT_AUTORISAT_MAX 65499

# define MAX_CHAR 9
# define MAX_CHAR_TRONC 20
# define MAX_CMD_LENGTH 512
# define MAX_KICK_LIST 3
# define MAX_CHAN_NAME 50
# define MAX_JOIN_CHAN_AT_ONCE 3
# define MAX_CHAN_NB 10

# define JOKES_PATH "src/Command/Bot/.jokes"
# define CAT_PATH "src/Command/Bot/.cat"
# define CAR_PATH "src/Command/Bot/.car"
# define RABBIT_PATH "src/Command/Bot/.rabbit"
# define FACE_PATH "src/Command/Bot/.face"
# define BIKE_PATH "src/Command/Bot/.bike"


# define BN "\r\n"
# define SP " "

/* ******************** RPL Macro ******************** */
# define RPL_CONNECTED(nickname) (": 001 " + nickname + " : Welcome to the IRC server!" + BN)
# define RPL_UMODEIS(hostname, channel, mode, user) (":" + hostname + " MODE " + channel + " " + mode + " " + user + BN)
# define RPL_CREATIONTIME(nickname, channel, creationtime) (": 329 " + nickname + " #" + channel + " " + creationtime + BN)
# define RPL_CHANNELMODES(nickname, channel, modes) (": 324 " + nickname + " #" + channel + " " + modes + BN)
# define RPL_CHANGEMODE(hostname, channel, mode, arguments) (":" + hostname + " MODE #" + channel + " " + mode + " " + arguments + BN)
# define RPL_NICKCHANGE(oldnickname, nickname) (":" + oldnickname + " NICK " + nickname + BN)
# define RPL_NOTOPIC(nickname, channel) (": 331 " + nickname + " #" + channel + " :No topic is set" + BN)
# define RPL_TOPIC(nickname, channel, topic) (": 332 " + nickname + " #" +channel + " :" + topic + BN)
# define RPL_TOPICWHOTIME(nickname, channel, topic, timer) (": 333 " + nickname + " #" + channel + " TOPIC " + topic + " " + timer + BN)
# define RPL_INVITING(nickname, channel) (": 341 " + nickname + " :has been successfuly invited to #" + channel + BN)
# define RPL_LISTSTART(nickname) (":" + nickname + " 321 :Begin of /LIST" + BN)
# define RPL_LIST(channel, clientsnumber, topic) ("#" + channel + " " + clientsnumber + " " + topic + BN)
# define RPL_LISTEND(nickname) (":" + nickname + " 323 :End of /LIST" + BN)
# define RPL_NAMREPLY(nickname, channel, clientslist) (": 353 " + nickname + " @ #" + channel + " :" + clientslist + BN)
# define RPL_ENDOFNAMES(nickname, channel) (": 366 " + nickname + " #" + channel + " :END of /NAMES list" + BN)

/* ******************** IRC Error Macro ******************** */
# define ERR_INVALIDMODEPARM(channel, mode) (": 696 #" + channel + " Invalid mode parameter. " + mode + BN)
# define ERR_KEYSET(channel) (": 467 #" + channel + " Channel key already set. " + BN)
# define ERR_UNKNOWNMODE(nickname, channel, mode) (": 472 " + nickname + " #" + channel + " " + mode + " :is not a recognised channel mode" + BN)
# define ERR_NEEDMOREPARAMS(nickname) (": 461 " + nickname + " :Not enough parameters." + BN)
# define ERR_NOSUCHCHANNEL(nickname, channel) (": 403 " + nickname + " #" + channel + " :No such channel" + BN)
# define ERR_NOSUCHNICK(name) (": 401 " + name + " :No such nick" + BN )
# define ERR_INCORPASS(nickname) (": 464 " + nickname + " :Password incorrect !" + BN )
# define ERR_ALREADYREGISTERED(nickname) (": 462 " + nickname + " :You may not reregister !" + BN )
# define ERR_NONICKNAMEGIVEN(nickname) (": 431 " + nickname + " :No nickname given" + BN )
# define ERR_NICKNAMEINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + BN)
# define ERR_ERRONEUSNICKNAME(nickname) (": 432 " + nickname + " :Erroneus nickname" + BN)
# define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + BN)
# define ERR_CMDNOTFOUND(nickname, command) (": 421 " + nickname + " " + command + " :Unknown command" + BN)
# define ERR_NOTONCHANNEL(nickname, channel) (": 442 " + nickname + " #" + channel + " :You're not on that channel" + BN)
# define ERR_CHANOPRIVSNEEDED(nickname, channel) (": 482 " + nickname + " #" + channel + " :You're not channel operator" + BN)
# define ERR_USERONCHANNEL(nickname, channel) (": 443 " + nickname + " #" + channel + " :is already on channel" + BN)
# define ERR_INVITEONLYCHAN(nickname, channel) (": 473 " + nickname + " #" + channel + " :You're not invited to this channel (+i)" + BN)
# define ERR_CHANNELISFULL(nickname, channel) (": 471 " + nickname + " #" + channel + " :Cannot join channel (+l)" + BN)
# define ERR_INCORPASS(nickname) (": 464 " + nickname + " :Password incorrect !" + BN )
# define ERR_USERONCHANNEL(nickname, channel) (": 443 " + nickname + " #" + channel + " :is already on channel" + BN)
# define ERR_TOOMANYCHANNELS(nickname) (": 405 " + nickname + " :You have joined too many channels" + BN)
# define ERR_CANNOTSENDTOCHAN(nickname, channel) (": 404 " + nickname + " #" + channel + " :Cannot send to channel" + BN)
# define ERR_NOTEXTTOSEND(nickname) (": 412 " + nickname + " :No text to send" + BN)

/* ******************** Personal Macro ******************** */
# define RPL_INVITED(nickname, channel) (":" + nickname + " :invite you to #" + channel + BN)
# define ERR_MAXCMDLENGTH(cmd) (":" + cmd + " :command too long" + BN )
# define ERR_BADNICKNAME(nickname) (":" + nickname + " :bad nickname given" + BN )
# define ERR_BADPARAM(nickname) (":" + nickname + " :bad parameters given" + BN )
# define ERR_TOOMANYTARGETS(nickname) (":" + nickname + " :too many targets" + BN )
# define RPL_PART(nickname, channel, reason) (":" + nickname + " PART #" + channel + " " + reason + BN)
# define RPL_QUIT(nickname, channel, reason) (":" + nickname + " QUIT #" + channel + " :" + reason + BN)
# define RPL_MODE(nickname, channel, mode, param) (":" + nickname + " MODE #" + channel + " " + mode + " " + param + BN)
# define RPL_JOIN(nickname, channel) (":" + nickname + " JOIN #" + channel + BN)
# define ERR_SERVERFULL_CHAN(nickname) (":" + nickname + " :Server is full, no more channels can be created" + BN)
# define RPL_PRIVMSG(nickname, dest, msg) (":" + nickname + " PRIVMSG " + dest + " :" + msg + BN)
# define RPL_KICK(nickname, channel, target, reason) (":" + nickname + " KICK #" + channel + " " + target + " :" + reason + BN)
# define ERR_FILENOTFOUND(filename) (":" + filename + " :File not found" + BN)
# define ERR_CANNOTOPENFILE(filename) (":" + filename + " :Cannot open file" + BN)
# define RPL_WAITINGCONNECTION(nickname, port) (":" + nickname + " :Waiting for connection on port " + port + BN)
# define RPL_INVITECONNECTION(nickname, port) (":" + nickname + " :Invite you to connect on port " + port + BN)
# define RPL_ACCEPTEDCONNECTION(nickname, port) (":" + nickname + " :Accepted connection on port " + port + BN)
# define ERR_WRONGOPERATIONGIVEN(nickname, operator) (": " + nickname + " : " + operator + " : bad operator given" + BN)
# define ERR_DIVZERO(nickname) (": " + nickname + " : Division by zero is forbidden" + BN)

#endif
