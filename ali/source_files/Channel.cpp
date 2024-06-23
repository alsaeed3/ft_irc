/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmususa <tmususa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:50:49 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/23 18:06:42 by tmususa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <Channel.hpp>

Channel::Channel(std::string &channelName, Client &client) : channelName(channelName), topic(""), pass("")
{
	topic = "";
	pass = "";
	operators[client->getNick()] = client;
	users[client->getNick()] = client;
	//users.push_back(nickname);
	modes['i'] = false;
	modes['t'] = false;
	modes['k'] = false;
	modes['o'] = false;
	modes['l'] = false;
}


Channel &Server::getChannel(std:string channelName)
{
	std::map<std::string, Channel>::iterator chan = this->_channels(channelName);
	return chan->second;
}


// void	Channel::addClient( int client_fd ) {

// 	_clients.insert( client_fd );

// 	return;
// }

// void	Channel::removeClient( int client_fd ) {

// 	_clients.erase( client_fd );

// 	return;
// }

// void	Channel::broadcastMessage( const std::string &message, int sender_fd ) {

// 	for ( std::set<int>::iterator it = _clients.begin(); it != _clients.end(); ++it ) {

// 		int client_fd = *it;
// 		if ( client_fd != sender_fd ) {

// 			if ( send( client_fd, message.c_str(), message.length(), 0 ) == -1 ) {

// 				throw IrcException( "Can't send message to client" );
// 			}
// 		}
// 	}

// 	return;
// }

// Path: includes/Channel.hpp