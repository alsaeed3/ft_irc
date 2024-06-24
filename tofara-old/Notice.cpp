#include "Includes/Command.hpp"



//send notices to channels or to users, seperated by commas
void Command::noticeCommand(User user, std::string receivers, std::string message)
{
	//split the string based on comma
	//then iterate through resulting string if channel and if channel go through all the users and sent message
	//if not check if user and send to the user
}