/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 23:42:39 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/22 17:26:05 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include <IrcLibs.hpp>
#include <Reply.hpp>
#include <IrcException.hpp>
#include <ParseMessage.hpp>
#include <Client.hpp>
#include <Channel.hpp>

class Server {

	private:

		static const int						MAX_CLIENTS = FD_SETSIZE;
		static const int						BUFFER_SIZE = 1024;

		static int								_listeningSocket;
		static std::string						_serverPassword;
		static int								_serverPort;
		static std::string						_message;
		static struct sockaddr_in				_serverHint;
		static int								_hintLen;
		static char								_host[NI_MAXHOST];
		static char								_svc[NI_MAXSERV];
		static std::map<int, Client*>			_clients;
		
		static std::vector<pollfd>				_fds;

		Server( void );

		static void             setNonblocking(int fd);
		static void             handleNewConnection(void);
		static int     			ft_recv( int fd );
		static void				handleClientDisconnection(int client_fd, int bytesRecv);
		static void             handleClientMessage(int client_fd);
		static void             closeClient(int client_fd);
		// static void             authenticateClient(int client_fd, const std::string& password);
		static void				sendToClient( int client_fd );
		static void             cleanupServer(void);

		static void				processCommand( Client *client, const ParseMessage& parsedMsg);
		static int				registerConnection( Client* client, const ParseMessage& parsedMsg );

	public:

		std::map<std::string, Channel>	_channels;
		static void 			initServer(void);
		static void 			runServer(void);
		static void 			signalHandler(int signal);
		static void 			setServerPassword(const std::string& password) { _serverPassword = password; }
		static void 			setServerPort(int port) { _serverPort = port; }
};

#endif /* SERVER_HPP */