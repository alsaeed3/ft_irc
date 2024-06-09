/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:37:35 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/09 08:08:30 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include <vector>
#include "IrcException.hpp"
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>

class Client {

	private:

		int							_fd;
		std::string					_ip;
		std::string					_nickname;
		std::string					_username;
		bool						_authenticated;
		std::vector<std::string>	_channels;

	public:

		Client( int fd, const std::string &ip );
		void	sendMessage( const std::string &message );
		void	joinChannel( const std::string &channel );
		void	setAuthenticated( bool authenticated );
		bool	isAuthenticated( void ) const;
		void	setNickname( const std::string &nickname );
		void	setUsername( const std::string &username );
		std::string getFullIdentity( void ) const;
};

#endif /* CLIENT_HPP */