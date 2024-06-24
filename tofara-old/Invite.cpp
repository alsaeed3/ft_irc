#include "Command.hpp"


void Command::inviteCommand(User &user, std::string nickname, std::string channel)
{
	// Channel object
	//check if channel exists and starts with # or & else ERR_NOSUCHCHANNEL
	//that it is part of the channel and user not in channel already else ERR_NOTONCHANNEL
	//if in invite mode then check is user is an operator then add to invite list
	// if(channel->checkMode('i')) //function to return the corresponding channel object stored on server
	// {
	// 	if(!channel->isOperator(user->getNick()))
	// 	{
			// send error ERR_CHANOPRIVSNEEDED
			//return
	// 	}
	// }
	//if(channel->isInChannel(nickname))
	 	//send ERR_USERONCHANNEL
		// return;
	//finally add the nickname to the list of invites
	//send the message RPL_INVITING
}