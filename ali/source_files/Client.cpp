/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmususa <tmususa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:42:49 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/23 19:03:43 by tmususa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Client.hpp>

Client::Client( void ) :	_fd( 0 ),
							_isCorrectPassword( false ),
							_nickname( "" ),
							_username( "" ),
							_realname( "" ),
							_channel( "" ) {

	return;
}

Client::Client( int fd ) :	_fd( fd ), 
													_isCorrectPassword( false ),
													_nickname( "" ),
													_username( "" ),
													_realname( "" ),
													_channel( "" ) {

	return;
}

bool		Client::sendMessage( const std::string &message ) {

	if ( send( _fd, message.c_str(), message.size(), 0 ) == -1 ) {

		return false;
	}

	return true;
}

// void		Client::joinChannel( const std::string &channel ) {

	
// 	return;
// }


bool		Client::isRegistered( void ) const {

	return _isCorrectPassword && _username != "" && _nickname != "" && _realname != "";
}

void		Client::setIsCorrectPassword( bool isCorrectPassword ) {

	_isCorrectPassword = isCorrectPassword;

	return;
}

void		Client::setNickname( const std::string &nickname ) {

	_nickname = nickname;
	return;
}

void		Client::setUsername( const std::string &username ) {

	_username = username;
	return;
}

std::string	&Client::getNickname( void ) const 
{

	return _nickname;
}

std::string	Client::getUsername( void ) const {

	return _username;
}

// std::string	Client::getFullIdentity( void ) const {

// 	return _nickname + "!" + _username + "@" + _ip;
// }

// std::string	Client::getIp( void ) const {

// 	return _ip;
// }

// Path: includes/Client.hpp