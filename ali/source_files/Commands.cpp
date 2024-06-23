/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmususa <tmususa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 07:48:18 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/23 18:06:18 by tmususa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <Channel.hpp>

// int	Server::registerConnection( Client* client, const ParseMessage& parsedMsg ) {

// 	if ( client->isRegistered() == true ) {

// 		client->serverReplies.push_back( ERR_ALREADYREGISTRED);
// 		return 0;
// 	}
	
	
// 	return 0;
// }

// void Server::processCommand( Client *client, const ParseMessage& parsedMsg) {

// 	std::string command = parsedMsg.getCmd();

// 	if ( command == "QUIT" )
// 		quitCmd( parsedMsg.getTrailing(), client );	

	

// 	return;
// }


void Server::joinCommand(Client &client, std::string &channels, std::string &keys)
{
	// check if user and channel exist
	//
	std::vector<std::string> chan_list = ft_split(channels, ',');
	std::vector<std::string> key_list = ft_split(keys, ',');
	std::vector<std::string>::iterator itr_key;
	std::vector<std::string>::iterator itr_chan;

	// check if user exists.
	// iterate through the channels
	for (itr_chan = chan_list.begin(); itr_chan != chan_list.end(); ++itr_chan)
	{
		//check that the channel name starts with # or &
		// if(*itr_chan.at(0) != "#" || *itr_chan.at(0) != "&")
				//ERR_NOT_A_CHANNEL
		// if channel exists do all this below
		if(this->isChannelinServer(*itr_chan))
		{
			if()
		}
		else
		{
			
		}
		// if(*itr_chan.checkmode('i') == true) if mode is invite mode
		//check if user is in invite list , if not then print error message and continue
		//if so check if user mode is k and if so then use key to check with pass and increment, if key is wrong then continue
		//if channel correct then add user
		//if channel not exist create the channel with the new user
		else
		{
			Channel newChan()
			// this->_channels[*itr_chan] = 
		}
	}

}