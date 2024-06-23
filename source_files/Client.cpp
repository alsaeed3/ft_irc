/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamzaou <shamzaou@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:42:49 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/23 17:47:08 by shamzaou         ###   ########.fr       */
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

std::string	Client::getNickname( void ) const {

	return _nickname;
}

std::string	Client::getUsername( void ) const {

	return _username;
}

std::string	Client::getFullIdentity( void ) const {

	return _nickname + "!" + _username + "@" + _ip;
}

std::string	Client::getIp( void ) const {

	return _ip;
}

// Path: includes/Client.hpp



// changes by Salim

void Client::leaveChannel(const std::string &channel) {
    std::vector<std::string>::iterator it = std::find(_channels.begin(), _channels.end(), channel);
    if (it != _channels.end()) {
        _channels.erase(it);
    }
}

void Client::inviteToChannel(const std::string &channel) {
    if (std::find(_invitedChannels.begin(), _invitedChannels.end(), channel) == _invitedChannels.end()) {
        _invitedChannels.push_back(channel);
    }
}

bool Client::isInvited(const std::string &channel) const {
    return std::find(_invitedChannels.begin(), _invitedChannels.end(), channel) != _invitedChannels.end();
}

int Client::getFd() const {
    return _fd;
}
