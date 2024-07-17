/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:42:49 by alsaeed           #+#    #+#             */
/*   Updated: 2024/07/17 19:01:53 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Client::Client( void ) :	_fd( 0 ),
							_isCorrectPassword( false ),
							_nickname( "" ),
							_username( "" ),
							_channel( "" ) {
	memset(conRegi, 0, 3);
	isRegistered = false;
	return;
}

Client::Client( int fd ) :	_fd( fd ), 
													_isCorrectPassword( false ),
													_nickname( "" ),
													_username( "" ),
													_channel( "" ) {
	memset(conRegi, 0, 3);
	isRegistered = false;
	return;
}

bool		Client::sendMessage( const std::string &message ) {

	if ( send( _fd, message.c_str(), message.size(), 0 ) == -1 ) {

		return false;
	}

	return true;
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

	return const_cast<std::string &>(_nickname);
}

std::string	Client::getUsername( void ) const {

	return _username;
}

bool		Client::getIsCorrectPassword( void ) const {
		return _isCorrectPassword;
}

void	Client::setFd(int value)
{
	_fd = value;
}
int 	Client::getFd( void ) const
{
	return _fd;
}

bool Server::isUserInServer(std::string nickname)
{
	    return std::find(_nicknames.begin(),_nicknames.end(), nickname) != _nicknames.end();
}

Client	*Server::getClient(std::string nickname)
{
    std::map<int, Client*>::iterator it;
	
    for (it = _clients.begin(); it != _clients.end(); ++it) {
        Client* client = it->second;
        if (client->getNickname() == nickname) 
		{
            return client;
        }
    }
    return NULL;
}