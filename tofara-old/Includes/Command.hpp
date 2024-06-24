#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
//Do a forward declaration for the other commands in the 


class User;
class Channel;

class Command
{

	public:
		Command();
		~Command();

		//Only operator can eject(kick, changemode or invite if in invite mode or change topic if in t channle mode)
		//JOIN command
		void joinCommand(User &user, std::string &channels, std::string &keys);
		//KICK command
		void kickCommand(User &user, std::string channel, std::string removerUsers, std::string &comment);
		//INVITE
		void inviteCommand(User &user, std::string nickname, std::string channel);
		void topic();
		void privmsg();
		void mode();
		void part();
		void notice();
	
		//function to split the message passed
		//channel exist
		//user exist
};

std::vector<std::string>  ft_split(std::string string, char delimiter);
// std::vector<std::string> remove_spaces(std::string &str);

#endif