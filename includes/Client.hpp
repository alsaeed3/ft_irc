/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:37:35 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/19 18:43:32 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include <vector>
#include <IrcException.hpp>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>

class Client {

	private:

		int							_fd;
		bool						_isCorrectPassword;
		std::string					_ip;
		std::string					_nickname;
		std::string					_username;
		std::string					_realname;
		std::string					_channel;

	public:

		std::vector<std::string>	serverReplies;

		Client( void );
		Client( int fd, const std::string &ip );

		bool		sendMessage( const std::string &message );
		void		joinChannel( const std::string &channel );
		
		void		setIsCorrectPassword( bool isCorrectPassword );
		void		setNickname( const std::string &nickname );
		void		setUsername( const std::string &username );
		
		std::string getFullIdentity( void ) const;
		std::string getNickname( void ) const;
		std::string getUsername( void ) const;
		std::string getIp( void ) const;

		bool		isRegistered( void ) const;
};

#endif /* CLIENT_HPP */