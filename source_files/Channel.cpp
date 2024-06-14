/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:50:49 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/13 17:47:33 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Channel.hpp>

void	Channel::addClient( int client_fd ) {

	_clients.insert( client_fd );

	return;
}

void	Channel::removeClient( int client_fd ) {

	_clients.erase( client_fd );

	return;
}

void	Channel::broadcastMessage( const std::string &message, int sender_fd ) {

	for ( std::set<int>::iterator it = _clients.begin(); it != _clients.end(); ++it ) {

		int client_fd = *it;
		if ( client_fd != sender_fd ) {

			if ( send( client_fd, message.c_str(), message.length(), 0 ) == -1 ) {

				throw IrcException( "Can't send message to client" );
			}
		}
	}

	return;
}

// Path: includes/Channel.hpp