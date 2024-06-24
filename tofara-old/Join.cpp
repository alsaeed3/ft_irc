#include "Includes/Command.hpp"

// check that the channel is in invite mode,if so then you need to be invited before joining else error ERR_INIVITEONLYCHAN
// check for channel in key mode, // if so then must be a pass/key to join the channel
// if key is wrong then ERR_BADCHANNELKEY error, invalid key, so if mode i

void Command::joinCommand(User &user, std::string &channels, std::string &keys)
{
	// check if user and channel exist
	//
	std::vector<std::string> chan_list = ft_split(channels, ',');
	std::vector<std::string> key_list = ft_split(keys, ',');
	std::vector<std::string>::iterator itr_key;
	std::vector<std::string>::iterator itr_chan;

	// check if user exists.
	// iterate through the channels
	for(itr_chan = chan_list.begin(); itr_chan != chan_list.end(); ++itr_chan)
	{
		//check that the channel name starts with # or &
		// if channel exists do all this below
		// if(*itr_chan.checkmode('i') == true) if mode is invite mode
		//check if user is in invite list , if not then print error message and continue
		//if so check if user mode is k and if so then use key to check with pass and increment, if key is wrong then continue
		//if channel correct then add user
		//if channel not exist create the channel with the new user
	}

}