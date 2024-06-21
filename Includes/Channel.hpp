#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Command.hpp"

//forward declaration of of client
//delete when the last user leave
class User;

class Channel
{
	private:
		int totalUsers;
		std::string topic;
		std::string pass;
		std::vector<std::string> operators;
		std::vector<std::string> users;
		std::vector<std::string> inviteList;
		std::string &channelName;
		std::map<char, bool> modes;
		int userLimit;
		Channel();

	public:
	//constructor
		Channel(std::string &nickname, std::string &channelName);
		~Channel();
	//getters and setters
	//kickout the user function
		void kickUser(User &user);
		void addUser(User &user);
		void setTopic(std::string &topic);
		bool isInChannel(std::string &nickname);
		bool isOperator(std::string &nickname);
		std::vector<std::string> getUsers();
		bool checkMode(char c);
		void addInvite(std::string &invite);
		void setPass(std::string &password);
		std::string getPass();

	//delete the channel when there is no user left
	//changeTopic --> notify all users when there is a change
	//setMode function

};

#endif