#include "Command.hpp"


//not enough params, no such channel, chanpriviledges, usernot in channel, user is part of channel, both need to be part of channel
void Command::kick(User user, std::string channel, std::string removerUsers, std::string comment)
{
	//if channel exists
	//split the users by comma
	//run a for loop for each user, checking if they exist
	//then sent the user a message saying you have been kicked out. Also check you need the for priviledges
}