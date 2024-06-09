/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 23:42:42 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/09 11:10:15 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server( int port, const std::string &password ) : _serverPassword( password ) {

	try {

		initServer( port );
	}
	catch ( const IrcException &e ) {

		std::cerr << "Server initialization error: " << e.what() << std::endl;
		exit( 1 );
	}
	
	return;
}

void Server::initServer( int port ) {

	_listeningSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( _listeningSocket == -1 ) {

		throw IrcException( "Can't create a socket!" );
	}

	sockaddr_in serverHint;
	std::memset( &serverHint, 0, sizeof( serverHint ) );
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons( port );
	serverHint.sin_addr.s_addr = INADDR_ANY;

	if ( bind( _listeningSocket, (sockaddr*)&serverHint, sizeof( serverHint ) ) == -1 ) {

		throw IrcException( "Can't bind to IP/port" );
	}

	if ( listen( _listeningSocket, SOMAXCONN ) == -1 ) {
		
		throw IrcException( "Can't listen!" );
	}
	
	setNonblocking( _listeningSocket );
	_fds.push_back( { _listeningSocket, POLLIN, 0 } );

	return;
}

void Server::setNonblocking( int fd ) {

	int flags = fcntl( fd, F_GETFL, 0 );
	if ( flags == -1 ) {

		throw IrcException( "Can't get file descriptor flags" );
	}

	if ( fcntl( fd, F_SETFL, flags | O_NONBLOCK ) == -1 ) {

		throw IrcException( "Can't set file descriptor flags" );
	}

	return;
}

void Server::runServer( void ) {

	while ( true ) {

		if ( poll( _fds.data(), _fds.size(), -1 ) == -1 ) {

			throw IrcException( "Poll error" );
		}

		for ( int i = 0; i < static_cast<int>( _fds.size() ); ++i ) {

			if ( _fds[i].revents & POLLIN ) {

				if ( _fds[i].fd == _listeningSocket ) {

					handleNewConnection();
				}
				else {

					handleClientMessage( _fds[i].fd );
				}
			}
		}
		
		_fds.erase( std::remove_if( _fds.begin(), _fds.end(), []( const pollfd &fd ) { return fd.revents & POLLHUP; } ), _fds.end() );
	}

	close( _listeningSocket );
	return;
}

void Server::handleNewConnection( void ) {

	sockaddr_in clientHint;
	socklen_t clientSize = sizeof( clientHint );
	int clientSocket = accept( _listeningSocket, (sockaddr*)&clientHint, &clientSize );
	if ( clientSocket == -1 ) {

		std::cerr << "Problem with client connecting!" << std::endl;
		return;
	}

	try {
		
		setNonblocking( clientSocket );
		clients[clientSocket] = new Client( clientSocket, inet_ntoa( clientHint.sin_addr ) );
		_fds.push_back( { clientSocket, POLLIN, 0 } );
		std::cout << "New connection from " << inet_ntoa( clientHint.sin_addr ) << std::endl;
	} catch ( const IrcException &e ) {

		std::cerr << "Error handling new connection: " << e.what() << std::endl;
		close( clientSocket );
	}

	return;
}

void Server::handleClientMessage( int client_fd ) {

	char buffer[BUFFER_SIZE];
	std::memset( buffer, 0, BUFFER_SIZE );
	
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

void Server::processCommand( int client_fd, const std::string &command ) {

    Client *client = _clients[client_fd];
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
				Channel& channel = _channels[channelName];
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
					_channels[channelName].broadcastMessage( ":" + client->getFullIdentity() + " PRIVMSG " + channelName + " :" + message, client_fd );
				}
			}
			break;
		default:
			client->sendMessage( "ERROR: Unknown command" );
			break;
	}
}

void Server::closeClient( int client_fd ) {

	close( client_fd );
	delete _clients[client_fd];
	_clients.erase( client_fd );
	
	for ( auto& [channelName, channel] : _channels ) {

		channel.removeClient( client_fd );
	}

	for ( int i = 0; i < static_cast<int>( _fds.size() ); ++i ) {

		if ( _fds[i].fd == client_fd ) {

			_fds[i].fd = -1;
			break;
		}
	}

	return;
}

void Server::authenticateClient( int client_fd, const std::string &password) {

	if (password == _serverPassword) {

		_clients[client_fd]->setAuthenticated( true );
	} else {

		_clients[client_fd]->sendMessage("ERROR: Invalid password");
		closeClient( client_fd );
	}

	return;
}

// Path: includes/Server.hpp