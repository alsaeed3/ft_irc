/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 23:42:42 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/12 19:28:15 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

void Server::initServer(void) {

    Server::_listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (Server::_listeningSocket == -1) {

        throw IrcException("Can't create a socket!");
    }

	if ( fcntl( Server::_listeningSocket, F_SETFL, O_NONBLOCK ) == -1 ) {
	
		throw IrcException("Can't set file descriptor flags");
	}

    // memset(&Server::_serverHint, 0, sizeof(Server::_serverHint));
    Server::_serverHint.sin_family = AF_INET;
    Server::_serverHint.sin_addr.s_addr = INADDR_ANY;
    Server::_serverHint.sin_port = htons(Server::_serverPort);

	int opt = 1;
    if (setsockopt(Server::_listeningSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		
		close(Server::_listeningSocket);
        throw IrcException("Can't set socket options");
    }

    if (bind(Server::_listeningSocket, (struct sockaddr*)&Server::_serverHint, sizeof(Server::_serverHint)) == -1) {
        perror("bind");
		close(Server::_listeningSocket);
        throw IrcException("Can't bind to IP/port");
    }

    if (listen(Server::_listeningSocket, 1) == -1) {
        perror("listen");
		close(Server::_listeningSocket);
        throw IrcException("Can't listen!");
    }

	Server::_hintLen = sizeof(Server::_serverHint);

    gethostname(Server::_host, NI_MAXHOST);
    std::cout << "IRC server Listening on " << Server::_host << " on port " << Server::_serverPort << std::endl;
    std::cout << "Waiting for incoming connections..." << std::endl;
 
    Server::_fds.push_back({Server::_listeningSocket, POLLIN, 0});
}


void Server::setNonblocking(int fd) {

    if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
        Server::cleanupServer();
        throw IrcException("Can't set file descriptor flags");
    }
}

void    Server::runServer(void) {

    signal( SIGINT, Server::signalHandler );
    signal( SIGQUIT, Server::signalHandler );
    while (Server::_interruptSignal == false) {

        int pollCount = poll((&Server::_fds[0]), Server::_fds.size(), 1000);
        if (pollCount == -1) {

            Server::cleanupServer();
            throw IrcException("Poll error");
        }

        if ( Server::_fds[0].revents & POLLIN ) {

            handleNewConnection();
        }

        for (std::vector<pollfd>::iterator it = Server::_fds.begin(); it != Server::_fds.end(); ++it) {

            if (it->fd != Server::_listeningSocket && it->revents & POLLIN) {

                    handleClientMessage(it->fd);
            }
        }

        Server::_fds.erase(std::remove_if(Server::_fds.begin(), Server::_fds.end(), [](const pollfd& fd) { return fd.fd == -1; }), Server::_fds.end());
    }

    Server::cleanupServer();
    return;
}

void Server::handleNewConnection(void) {

    sockaddr_in clientHint;
    socklen_t clientSize = sizeof(clientHint);

    int clientSocket = accept(Server::_listeningSocket, (sockaddr*)&clientHint, &clientSize);
    if (clientSocket == -1) {

		if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // Resource temporarily unavailable, just continue the loop
            return;
        } else {
            perror("accept");
            throw IrcException("Can't accept client connection");
        }
    }

    int result = getnameinfo((sockaddr*)&clientHint, clientSize, Server::_host, NI_MAXHOST, Server::_svc, NI_MAXSERV, 0);
    if (result) {
        std::cout <<  Server::_host << " connected on " << Server::_svc << std::endl;
    } else {
        inet_ntop(AF_INET, &clientHint.sin_addr, Server::_host, NI_MAXHOST);
        std::cout <<  Server::_host << " connected on " << ntohs(clientHint.sin_port) << std::endl;
    }

    Client* tmpClient = new Client(clientSocket, inet_ntoa(clientHint.sin_addr));
    Server::_clients[clientSocket] = tmpClient;
    Server::_fds.push_back({clientSocket, POLLIN | POLLOUT, 0});
    setNonblocking(clientSocket);
}

void Server::handleClientMessage(int client_fd) {

    try {

        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);

        int bytesRecv = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (bytesRecv <= 0) {
            if (bytesRecv == 0) {
                std::cout << "Client " << client_fd << " disconnected" << std::endl;
            } else {
                std::cerr << "Error receiving message from client " << client_fd << " (" << strerror(errno) << ")" << std::endl;
            }
            closeClient(client_fd);
            return;
        } else if (bytesRecv > 0) {

            std::string message(buffer, bytesRecv);
            std::cout << "Received message from client " << client_fd << ": " << message << std::endl;
            processCommand(client_fd, message);
        }

    } catch (const IrcException& e) {
        std::cerr << "Error processing command from client " << client_fd << ": " << e.what() << std::endl;
    }
}

void Server::processCommand(int client_fd, const std::string& command) {
    Client* client = Server::_clients[client_fd];
    CommandType commandType = getCommandType(command);

    switch (commandType) {
        case CMD_PASS:
            authenticateClient(client_fd, command.substr(5));
            break;
        case CMD_NICK:
            if (!client->isAuthenticated()) {
                client->sendMessage("ERROR: You must authenticate first");
                break;
            }
            client->setNickname(command.substr(5));
            break;
        case CMD_USER:
            if (!client->isAuthenticated()) {
                client->sendMessage("ERROR: You must authenticate first");
                break;
            }
            client->setUsername(command.substr(5));
            break;
        case CMD_JOIN:
            if (!client->isAuthenticated()) {
                client->sendMessage("ERROR: You must authenticate first");
                break;
            }
            {
                std::string channelName = command.substr(5);
                Channel& channel = Server::_channels[channelName];
                channel.addClient(client_fd);
                client->joinChannel(channelName);
                client->sendMessage(":" + client->getFullIdentity() + " JOIN :" + channelName);
            }
            break;
        case CMD_PRIVMSG:
            if (!client->isAuthenticated()) {
                client->sendMessage("ERROR: You must authenticate first");
                break;
            }
            {
                std::size_t pos = command.find(' ', 8);
                if (pos != std::string::npos) {
                    std::string channelName = command.substr(8, pos - 8);
                    std::string message = command.substr(pos + 1);
                    Server::_channels[channelName].broadcastMessage(":" + client->getFullIdentity() + " PRIVMSG " + channelName + " :" + message, client_fd);
                }
            }
            break;
        default:
            client->sendMessage("ERROR: Unknown command");
            break;
    }
}

void Server::closeClient(int client_fd) {
    std::map<int, Client*>::iterator it = Server::_clients.find(client_fd);
    if (it != Server::_clients.end()) {
        close(client_fd);
        delete it->second;
        Server::_clients.erase(it);
    }

    for (std::map<std::string, Channel>::iterator it = Server::_channels.begin(); it != Server::_channels.end(); ++it)
        it->second.removeClient(client_fd);

    for (int i = 0; i < static_cast<int>(Server::_fds.size()); ++i) {
        if (Server::_fds[i].fd == client_fd) {
            Server::_fds[i].fd = -1;
            break;
        }
    }
}

void Server::authenticateClient(int client_fd, const std::string& password) {
    if (password == Server::_serverPassword) {
        Server::_clients[client_fd]->setAuthenticated(true);
    } else {
        Server::_clients[client_fd]->sendMessage("ERROR: Invalid password");
        closeClient(client_fd);
    }
}

void Server::signalHandler(int signal) {

    std::cerr << "Interrupt Signal (" << signal << ") received, Shutting down the Server..." << std::endl;

    Server::_interruptSignal = true;

    return;
}

void Server::cleanupServer(void) {
    for (std::vector<pollfd>::iterator it = Server::_fds.begin(); it != Server::_fds.end(); ++it)
        close(it->fd);

    for (std::map<int, Client*>::iterator it = Server::_clients.begin(); it != Server::_clients.end(); ++it)
        delete it->second;

    shutdown(Server::_listeningSocket, SHUT_RDWR);
    close(Server::_listeningSocket);
    Server::_fds.clear();
    Server::_clients.clear();
    Server::_channels.clear();
}

int Server::_listeningSocket = -1;
bool Server::_interruptSignal = false;
std::string Server::_serverPassword = "";
int Server::_serverPort = -1;
struct sockaddr_in Server::_serverHint;
int Server::_hintLen = 0;
char Server::_host[NI_MAXHOST];
char Server::_svc[NI_MAXSERV];
std::map<int, Client*> Server::_clients;
std::map<std::string, Channel> Server::_channels;
std::vector<pollfd> Server::_fds;

// Path: includes/Server.hpp