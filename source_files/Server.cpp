/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 23:42:42 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/11 06:03:02 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

void	Server::initServer( void ) {

	Server::_listeningSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( Server::_listeningSocket == -1 ) {

		throw IrcException( "Can't create a socket!" );
	}

	sockaddr_in serverHint;
	std::memset( &serverHint, 0, sizeof( serverHint ) );
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons( Server::_serverPort );
	serverHint.sin_addr.s_addr = INADDR_ANY;

	if ( bind( Server::_listeningSocket, (sockaddr*)&serverHint, sizeof( serverHint ) ) == -1 ) {

		throw IrcException( "Can't bind to IP/port" );
	}

	if ( listen( Server::_listeningSocket, SOMAXCONN ) == -1 ) {
		
		throw IrcException( "Can't listen!" );
	}
	
	setNonblocking( Server::_listeningSocket );
	Server::_fds.push_back( { Server::_listeningSocket, POLLIN, 0 } );

	char	hostname[NI_MAXHOST];

	memset( hostname, 0, NI_MAXHOST );

	gethostname( hostname, NI_MAXHOST );
	std::cout << "IRC server Listening on " << hostname << " on port " << Server::_serverPort << std::endl;
	std::cout << "Waiting for incoming connections..." << std::endl;

	return;
}

void	Server::setNonblocking( int fd ) {

	int flags = fcntl( fd, F_GETFL, 0 );
	if ( flags == -1 ) {

		throw IrcException( "Can't get file descriptor flags" );
	}

	if ( fcntl( fd, F_SETFL, flags | O_NONBLOCK ) == -1 ) {

		throw IrcException( "Can't set file descriptor flags" );
	}

	return;
}

void	Server::runServer( void ) {

	while ( true ) {

		if ( poll( Server::_fds.data(), Server::_fds.size(), -1 ) == -1 ) {

			throw IrcException( "Poll error" );
		}

		for ( int i = 0; i < static_cast<int>( Server::_fds.size() ); ++i ) {

			if ( Server::_fds[i].revents & POLLIN ) {

				if ( Server::_fds[i].fd == Server::_listeningSocket ) {

					handleNewConnection();
				}
				else {

					handleClientMessage( Server::_fds[i].fd );
				}
			}
		}
		
		Server::_fds.erase( std::remove_if( Server::_fds.begin(), Server::_fds.end(), []( const pollfd &fd ) { return fd.revents & POLLHUP; } ), Server::_fds.end() );
	}

	return;
}

void	Server::handleNewConnection( void ) {

	sockaddr_in clientHint;
	socklen_t clientSize = sizeof( clientHint );
	int clientSocket = accept( Server::_listeningSocket, (sockaddr*)&clientHint, &clientSize );
	if ( clientSocket == -1 ) {

		throw IrcException( "Can't accept client connection" );
	}
	std::cout << "Client socket: " << clientSocket << std::endl;

	Client* tmpClient = NULL;
	try {
		
		setNonblocking( clientSocket );
		char host[NI_MAXHOST];
		char service[NI_MAXSERV];
	
		memset( host, 0, NI_MAXHOST );
		memset( service, 0, NI_MAXSERV );
	
		int result = getnameinfo( (sockaddr*)&clientHint, sizeof( clientHint ), host, NI_MAXHOST, service, NI_MAXSERV, 0 );
		if ( result ) {

			std::cout << host << " connected on " << service << std::endl;
		} else {

			inet_ntop( AF_INET, &clientHint.sin_addr, host, NI_MAXHOST );
			std::cout << host << " connected on " << ntohs( clientHint.sin_port ) << std::endl;
		}

		tmpClient = new Client( clientSocket, inet_ntoa( clientHint.sin_addr ) );
		Server::_fds.push_back( { clientSocket, POLLIN, 0 } );
		Server::_clients[clientSocket] = tmpClient;
	} catch ( const IrcException &e ) {

		std::cerr << "Error handling new connection: " << e.what() << std::endl;
		delete tmpClient;
		close( clientSocket );
	}

	return;
}

void	Server::handleClientMessage( int client_fd ) {

	char buffer[BUFFER_SIZE];
	memset( buffer, 0, BUFFER_SIZE );
	
	int bytesRecv = recv( client_fd, buffer, BUFFER_SIZE, 0 );
	if ( bytesRecv <= 0 ) {
		
		if ( bytesRecv == 0 ) {

			std::cout << "Client " << client_fd << " disconnected" << std::endl;
		}
		else {

			std::cerr << "Error receiving message from client " << client_fd << std::endl;
		}
		
		closeClient( client_fd );

		return;
	}

	std::string message( buffer, bytesRecv );
	std::cout << "Received message from client " << client_fd << ": " << message << std::endl;

	try {
		
		processCommand( client_fd, message );
	} catch ( const IrcException &e ) {

		std::cerr << "Error processing command from client " << client_fd << ": " << e.what() << std::endl;
	}

	return;
}

void	Server::processCommand( int client_fd, const std::string &command ) {

    Client *client = Server::_clients[client_fd];
	CommandType commandType = getCommandType( command );

	switch ( commandType ) {

		case CMD_PASS:
			authenticateClient( client_fd, command.substr( 5 ) );
			break;
		case CMD_NICK:
			if ( !client->isAuthenticated() ) {

				client->sendMessage( "ERROR: You must authenticate first" );
				break;
			}
			client->setNickname( command.substr( 5 ) );
			break;
		case CMD_USER:
			if ( !client->isAuthenticated() ) {

				client->sendMessage( "ERROR: You must authenticate first" );
				break;
			}
			client->setUsername( command.substr( 5 ) );
			break;
		case CMD_JOIN:
			if ( !client->isAuthenticated() ) {

				client->sendMessage( "ERROR: You must authenticate first" );
				break;
			}
			{
				std::string channelName = command.substr( 5 );
				Channel& channel = Server::_channels[channelName];
				channel.addClient( client_fd );
				client->joinChannel( channelName );
				client->sendMessage(":" + client->getFullIdentity() + " JOIN :" + channelName);
			}
			break;
		case CMD_PRIVMSG:
			if ( !client->isAuthenticated() ) {

				client->sendMessage( "ERROR: You must authenticate first" );
				break;
			}
			{
				std::size_t pos = command.find( ' ', 8 );
				if ( pos != std::string::npos ) {
					
					std::string channelName = command.substr( 8, pos - 8 );
					std::string message = command.substr( pos + 1 );
					Server::_channels[channelName].broadcastMessage( ":" + client->getFullIdentity() + " PRIVMSG " + channelName + " :" + message, client_fd );
				}
			}
			break;
		default:
			client->sendMessage( "ERROR: Unknown command" );
			break;
	}
}

void	Server::closeClient( int client_fd ) {

	std::map<int, Client*>::iterator it = Server::_clients.find( client_fd );
	if ( it != Server::_clients.end() ) {

		close( client_fd );
		delete it->second;
		Server::_clients.erase( it );
	}

	for (std::map<std::string, Channel>::iterator it = Server::_channels.begin(); it != Server::_channels.end(); ++it)
		it->second.removeClient( client_fd );

	for ( int i = 0; i < static_cast<int>( Server::_fds.size() ); ++i ) {

		if ( Server::_fds[i].fd == client_fd ) {

			Server::_fds[i].fd = -1;
			break;
		}
	}

	return;
}

void	Server::authenticateClient( int client_fd, const std::string &password) {

	if (password == Server::_serverPassword) {

		Server::_clients[client_fd]->setAuthenticated( true );
	} else {

		Server::_clients[client_fd]->sendMessage("ERROR: Invalid password");
		closeClient( client_fd );
	}

	return;
}

void	Server::signalHandler( int signal ) {

	std::cerr << "Signal " << signal << " received" << std::endl;
	
	for ( std::vector<pollfd>::iterator it = Server::_fds.begin(); it != Server::_fds.end(); ++it )
		close( it->fd );

	for (std::map<int, Client*>::iterator it = Server::_clients.begin(); it != Server::_clients.end(); ++it)
        delete it->second;

	shutdown( Server::_listeningSocket, SHUT_RDWR );
	close( Server::_listeningSocket );
	Server::_fds.clear();
	Server::_clients.clear();
	Server::_channels.clear();
	
	exit ( signal );
}

int								Server::_listeningSocket = -1;
std::string 					Server::_serverPassword = "";
int								Server::_serverPort = -1;
std::map<int, Client*>			Server::_clients;
std::map<std::string, Channel>	Server::_channels;
std::vector<pollfd>				Server::_fds;

// Path: includes/Server.hpp