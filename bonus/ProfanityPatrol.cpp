/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProfanityPatrol.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamzaou@student.42abudhabi.ae <shamzaou>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:49:41 by shamzaou@student  #+#    #+#             */
/*   Updated: 2024/07/10 20:20:16 by shamzaou@student ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ProfanityPatrol.hpp"
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

ProfanityPatrol::ProfanityPatrol(const std::string& nickname, const std::string& username)
    : _nickname(nickname), _username(username), _socket(-1), _currentChannel("") {
    loadBadWords("bad_words.txt");
}

ProfanityPatrol::~ProfanityPatrol() {
    if (_socket != -1) {
        close(_socket);
    }
}

void ProfanityPatrol::loadBadWords(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file) {
        std::cerr << "Error opening bad words file: " << filename << std::endl;
        return;
    }
    std::string word;
    while (file >> word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        _badWords.push_back(word);
    }
}

bool ProfanityPatrol::containsBadWord(const std::string& message) const {
    std::string lowerMessage = message;
    std::transform(lowerMessage.begin(), lowerMessage.end(), lowerMessage.begin(), ::tolower);
    for (std::vector<std::string>::const_iterator it = _badWords.begin(); it != _badWords.end(); ++it) {
        if (lowerMessage.find(*it) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void ProfanityPatrol::run() {
    if (!start("localhost", 6667, "your_password_here")) {
        std::cerr << "Failed to start the bot" << std::endl;
        return;
    }

    while (true) {
        fd_set readfds;
        struct timeval tv;
        int max_fd = _socket + 1;

        FD_ZERO(&readfds);
        FD_SET(_socket, &readfds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        int activity = select(max_fd, &readfds, NULL, NULL, &tv);
        if (activity < 0) {
            std::cerr << "Select error" << std::endl;
            break;
        }

        if (FD_ISSET(_socket, &readfds)) {
            if (!processMessages()) {
                break;
            }
        }
    }
}

// Implement start() method to connect and login
bool ProfanityPatrol::start(const std::string& host, int port, const std::string& password) {
    if (!connectToServer(host, port)) {
        return false;
    }
    if (!login(password)) {
        return false;
    }
    return true;
}

// ... (other methods remain the same)

bool ProfanityPatrol::connectToServer(const std::string& host, int port) {
    struct sockaddr_in server_addr;
    struct hostent *server;

    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return false;
    }

    server = gethostbyname(host.c_str());
    if (server == NULL) {
        std::cerr << "Error, no such host: " << host << std::endl;
        return false;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    server_addr.sin_port = htons(port);

    if (connect(_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        return false;
    }

    fcntl(_socket, F_SETFL, O_NONBLOCK);
    std::cout << "Connected to server" << std::endl;
    return true;
}

bool ProfanityPatrol::login(const std::string& password) {
    std::string pass_cmd = "PASS " + password + "\r\n";
    std::string nick_cmd = "NICK " + _nickname + "\r\n";
    std::string user_cmd = "USER " + _username + " 0 * :Profanity Patrol Bot\r\n";

    if (send(_socket, pass_cmd.c_str(), pass_cmd.length(), 0) < 0 ||
        send(_socket, nick_cmd.c_str(), nick_cmd.length(), 0) < 0 ||
        send(_socket, user_cmd.c_str(), user_cmd.length(), 0) < 0) {
        std::cerr << "Error sending login information" << std::endl;
        return false;
    }

    std::cout << "Sent login information" << std::endl;
    return true;
}

void ProfanityPatrol::joinChannel(const std::string& channel) {
    std::string join_cmd = "JOIN " + channel + "\r\n";
    if (send(_socket, join_cmd.c_str(), join_cmd.length(), 0) < 0) {
        std::cerr << "Error joining channel: " << channel << std::endl;
        return;
    }
    _currentChannel = channel;
    std::cout << "Joined channel: " << channel << std::endl;
}

void ProfanityPatrol::sendMessage(const std::string& message) {
    if (_currentChannel.empty()) {
        std::cerr << "Not in a channel" << std::endl;
        return;
    }

    std::string privmsg = "PRIVMSG " + _currentChannel + " :" + message + "\r\n";
    if (send(_socket, privmsg.c_str(), privmsg.length(), 0) < 0) {
        std::cerr << "Error sending message" << std::endl;
    }
}

void ProfanityPatrol::kickUser(const std::string& channel, const std::string& user, const std::string& reason) {
    std::string kick_cmd = "KICK " + channel + " " + user + " :" + reason + "\r\n";
    if (send(_socket, kick_cmd.c_str(), kick_cmd.length(), 0) < 0) {
        std::cerr << "Error kicking user " << user << " from " << channel << std::endl;
        return;
    }
    std::cout << "Kicked user " << user << " from " << channel << std::endl;
}

bool ProfanityPatrol::processMessages() {
    char buffer[4096];
    int bytesReceived = recv(_socket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::istringstream iss(buffer);
        std::string line;
        
        while (std::getline(iss, line)) {
            if (line.find("PING") == 0) {
                std::string pong = "PONG" + line.substr(4) + "\r\n";
                send(_socket, pong.c_str(), pong.length(), 0);
            }
            else if (line.find("INVITE") != std::string::npos) {
                size_t channelPos = line.find('#');
                if (channelPos != std::string::npos) {
                    std::string channel = line.substr(channelPos);
                    channel = channel.substr(0, channel.find(' '));
                    joinChannel(channel);
                }
            }
            else if (line.find("PRIVMSG") != std::string::npos) {
                size_t colonPos = line.find(" :");
                if (colonPos != std::string::npos) {
                    std::string message = line.substr(colonPos + 2);
                    std::string sender = line.substr(1, line.find('!') - 1);
                    std::string target = line.substr(line.find("PRIVMSG") + 8);
                    target = target.substr(0, target.find(' '));

                    if (containsBadWord(message)) {
                        kickUser(target, sender, "Profanity is not allowed");
                        sendMessage(target + " :User " + sender + " has been ejected from the server for obscene language.");
                    }
                }
            }
        }
    }
    else if (bytesReceived == 0) {
        std::cout << "Disconnected from server" << std::endl;
        return false;
    }
    else if (bytesReceived < 0) {
        if (errno != EWOULDBLOCK && errno != EAGAIN) {
            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
            return false;
        }
    }
    return true;
}