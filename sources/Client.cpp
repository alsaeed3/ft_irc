/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:42:49 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/08 11:47:31 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

// Constructor
Client::Client( int fd, const std::string &ip ) :	_fd( fd ), 
													_ip( ip ), 
													_authenticated( false ) {

	return;
}

void		Client::sendMessage( const std::string &message ) {

	if ( send( _fd, message.c_str(), message.size(), 0 ) == -1 ) {

		throw IrcException( "Can't send message to client" );
	}

	return;
}

void		Client::joinChannel( const std::string &channel ) {

	_channels.push_back( channel );
	return;
}

void		Client::setAuthenticated( bool authenticated ) {

	_authenticated = authenticated;
	return;
}

bool		Client::isAuthenticated( void ) const {

	return _authenticated;
}

void		Client::setNickname( const std::string &nickname ) {

	_nickname = nickname;
	return;
}

void		Client::setUsername( const std::string &username ) {

	_username = username;
	return;
}

std::string	Client::getFullIdentity( void ) const {

	return _nickname + "!" + _username + "@" + _ip;
}

// Path: includes/Client.hpp