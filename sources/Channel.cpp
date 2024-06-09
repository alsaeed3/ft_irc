/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:50:49 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/08 11:55:00 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

void	Channel::addClient( int client_fd ) {

	_clients.insert( client_fd );

	return;
}

void	Channel::removeClient( int client_fd ) {

	_clients.erase( client_fd );

	return;
}

void	Channel::broadcastMessage( const std::string &message, int sender_fd ) {

	for ( int client_fd : _clients ) {

		if ( client_fd != sender_fd ) {

			if ( send( client_fd, message.c_str(), message.length(), 0 ) == -1 ) {

				throw IrcException( "Can't send message to client" );
			}
		}
	}

	return;
}

// Path: includes/Channel.hpp