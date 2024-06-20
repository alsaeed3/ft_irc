#include "Includes/Command.hpp"

// +i for the channel to tell that you need to be invited before you can join
//+k set or remove password on channel, must be operator to do this
//+t if enabled the channel must be be an operator to change the topic, else anyone can set topic, does not take any value
//+o set or remove operators in the channel, need to check if the user is an operator in the first place
//+l set or remove the user limit in the channel, need to be an operator to do this

void CommandMode(User user, std::string channel, std::string)
{
	//check if the channel exists and note that channel name should start with #
	//check modestring which is +i, if it is a valid
	//the user must have appropriate permissions to change or set the mode
	//
}