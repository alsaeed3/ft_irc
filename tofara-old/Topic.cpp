#include "Includes/Command.hpp"


void Command::Topic(User user, std::string channel, std::string topic)
{
	//check number of parameters
	//also check that the channel name starts with #test
	//check if channel exists
	//check if user is in channel
	//check if t mode is on then the user must be an operator to change the topic
	//empty string means remove the topic
	//go through every user in the channel and alert them the topic has changed. 
	//if paremeters are just two then e.g TOPIC #test then it means that we have to return the topic
	//also the third argument must start with :
}