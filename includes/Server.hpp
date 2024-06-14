/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 23:42:39 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/13 17:56:09 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

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
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <IrcException.hpp>
#include <Client.hpp>
#include <Channel.hpp>
#include <CommandType.hpp>

class Client;
class Channel;
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
	static std::map<std::string, Channel>	_channels;
	static std::vector<pollfd>				_fds;

	Server(void);

	static void             setNonblocking(int fd);
	static void             handleNewConnection(void);
	static int     			ft_recv( int fd );
	static void				handleClientDisconnection(int client_fd, int bytesRecv);
	static void             handleClientMessage(int client_fd);
	static void             closeClient(int client_fd);
	static void             processCommand(int client_fd, const std::string& command);
	static void             authenticateClient(int client_fd, const std::string& password);
	static void             cleanupServer(void);

public:
	static void 			initServer(void);
	static void 			runServer(void);
	static void 			signalHandler(int signal);
	static void 			setServerPassword(const std::string& password) { _serverPassword = password; }
	static void 			setServerPort(int port) { _serverPort = port; }
};

#endif /* SERVER_HPP */