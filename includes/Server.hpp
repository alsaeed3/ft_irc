/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 23:42:39 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/09 08:03:51 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <poll.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "IrcException.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "CommandType.hpp"

class Server {

	private:

		int								_listeningSocket;
		std::string 					_serverPassword;
		std::map<int, Client*>			_clients;
		std::map<std::string, Channel>	_channels;
		std::vector<pollfd>				_fds;

		void 	initServer( int port );
		void 	setNonblocking( int fd );
		void 	handleNewConnection( void );
		void 	handleClientMessage( int client_fd );
		void 	closeClient( int client_fd );
		void	processCommand( int client_fd, const std::string &command );
		void	authenticateClient( int client_fd, const std::string &password);

	public:

		Server( int port, const std::string &password );
		void	run( void );
};

#endif /* SERVER_HPP */
