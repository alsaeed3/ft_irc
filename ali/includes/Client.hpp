/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmususa <tmususa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:37:35 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/23 17:59:21 by tmususa          ###   ########.fr       */
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
		// std::string					_ip;
		std::string					_nickname;
		std::string					_username;
		std::string					_realname;
		std::string					_channel;

	public:

		std::vector<std::string>	serverReplies;

		Client( void );
		Client( int fd );

		// bool		sendMessage( const std::string &message );
		void		joinChannel( const std::string &channel );
		void	leaveChannel( const std::string &channel );
		
		void		setIsCorrectPassword( bool isCorrectPassword );
		void		setNickname( const std::string &nickname );
		void		setUsername( const std::string &username );
		
		std::string getFullIdentity( void ) const;
		std::string getNickname( void ) const;
		std::string getUsername( void ) const;
		// std::string getIp( void ) const;
		int			getFd( void ) const;

		bool		isRegistered( void ) const;
};

#endif /* CLIENT_HPP */