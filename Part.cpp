



// Command: PART
//   Parameters: <channel>{,<channel>} [<reason>]
//remove the user from all the specified channels
void Command::part(User user, std::string channels, std::string reason)
// {
	//split the channels based on comma
	//then check for each channel that first character is & or #
	//check if channel exists, if they are in there and then kick them out
	//errors are need more params, nosuchchannel, notinchannel

// }