/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProfanityPatrol.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamzaou@student.42abudhabi.ae <shamzaou>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:49:41 by shamzaou@student  #+#    #+#             */
/*   Updated: 2024/07/10 17:56:35 by shamzaou@student ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ProfanityPatrol.hpp"

ProfanityPatrol::ProfanityPatrol(const std::string& nickname, const std::string& username)
    : _nickname(nickname), _username(username), _socket(-1) {
    loadBadWords("bad_words.txt");
}

ProfanityPatrol::~ProfanityPatrol() {
    if (_socket != -1) {
        close(_socket);
    }
}

void ProfanityPatrol::loadBadWords(const std::string& filename) {
    std::ifstream file(filename.c_str());
    std::string word;
    while (file >> word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        _badWords.push_back(word);
    }
}

bool ProfanityPatrol::containsBadWord(const std::string& message) {
    std::string lowerMessage = message;
    std::transform(lowerMessage.begin(), lowerMessage.end(), lowerMessage.begin(), ::tolower);
    for (std::vector<std::string>::const_iterator it = _badWords.begin(); it != _badWords.end(); ++it) {
        if (lowerMessage.find(*it) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void ProfanityPatrol::processMessage(const std::string& message) {
    // TODO: Implement IRC message parsing
    // For now, let's assume the message is just the content
    if (containsBadWord(message)) {
        // TODO: Extract actual username from IRC message
        std::string offender = "SomeUser";
        kickUser(offender, "Inappropriate language");
        sendMessage(_currentChannel + " :User " + offender + " has been ejected from the server for obscene language.");
    }
}

void ProfanityPatrol::start(const std::string& host, int port, const std::string& password) {
    connectToServer(host, port);
    login(password);
    run();
}

void ProfanityPatrol::run() {
    char buffer[4096];
    while (true) {
        int bytesReceived = recv(_socket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::string message(buffer);
            
            // TODO: Handle PING messages to keep the connection alive
            
            if (message.find("INVITE " + _nickname) != std::string::npos) {
                // Extract channel name and join
                size_t pos = message.find('#');
                if (pos != std::string::npos) {
                    std::string channel = message.substr(pos);
                    channel = channel.substr(0, channel.find(' '));
                    joinChannel(channel);
                }
            } else if (message.find("PRIVMSG") != std::string::npos) {
                processMessage(message);
            }
        }
    }
}
