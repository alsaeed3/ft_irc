/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProfanityPatrol.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamzaou <shamzaou@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 12:28:50 by shamzaou          #+#    #+#             */
/*   Updated: 2024/07/12 12:46:40 by shamzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <netdb.h>  // For getaddrinfo
#include <cstring>  // For memset

class ProfanityPatrol {
private:
    std::string _address;
    int _port;
    std::string _password;
    int _socket;
    std::string _nickname;
    std::string _username;
    std::vector<std::string> _profanityList;

    // Private member functions
    bool connectToServer();
    void authenticate();
    void processMessages();
    void handleInvite(const std::string& channel);
    void scanForProfanity(const std::string& message, const std::string& channel);
    void sendMessage(const std::string& message);

public:
    ProfanityPatrol(const std::string& address, int port, const std::string& password);
    ~ProfanityPatrol();

    void run();
};


ProfanityPatrol::ProfanityPatrol(const std::string& address, int port, const std::string& password)
    : _address(address), _port(port), _password(password), _socket(-1) {
    _nickname = "ProfanityPatrol";
    _username = "profanitybot";
    // Initialize profanity list (you can expand this)
    _profanityList = {"badword1", "badword2", "badword3"};
}

ProfanityPatrol::~ProfanityPatrol() {
    if (_socket != -1) {
        close(_socket);
    }
}

bool ProfanityPatrol::connectToServer() {
    struct addrinfo hints, *result, *rp;
    int sfd;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          // Any protocol

    int s = getaddrinfo(_address.c_str(), std::to_string(_port).c_str(), &hints, &result);
    if (s != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(s) << std::endl;
        return false;
    }

    // Try each address until we successfully connect
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;  // Success

        close(sfd);
    }

    freeaddrinfo(result);  // Free the linked list

    if (rp == NULL) {  // No address succeeded
        std::cerr << "Could not connect" << std::endl;
        return false;
    }

    _socket = sfd;
    return true;
}

void ProfanityPatrol::authenticate() {
    sendMessage("PASS " + _password);
    sendMessage("NICK " + _nickname);
    sendMessage("USER " + _username + " 0 * :Profanity Patrol Bot");
}

void ProfanityPatrol::processMessages() {
    char buffer[4096];
    ssize_t bytesRead = recv(_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::istringstream iss(buffer);
        std::string line;
        while (std::getline(iss, line)) {
            if (line.find("PING") == 0) {
                sendMessage("PONG" + line.substr(4));
            } else if (line.find("INVITE") != std::string::npos) {
                size_t channelPos = line.rfind(':');
                if (channelPos != std::string::npos) {
                    std::string channel = line.substr(channelPos + 1);
                    handleInvite(channel);
                }
            } else if (line.find("PRIVMSG") != std::string::npos) {
                size_t colonPos = line.find(':', 1);
                if (colonPos != std::string::npos) {
                    std::string message = line.substr(colonPos + 1);
                    size_t spacePos = line.find(' ');
                    size_t channelPos = line.find('#', spacePos);
                    size_t endChannelPos = line.find(' ', channelPos);
                    if (channelPos != std::string::npos && endChannelPos != std::string::npos) {
                        std::string channel = line.substr(channelPos, endChannelPos - channelPos);
                        scanForProfanity(message, channel);
                    }
                }
            }
        }
    }
}

void ProfanityPatrol::handleInvite(const std::string& channel) {
    sendMessage("JOIN " + channel);
    std::cout << "Joined channel: " << channel << std::endl;
}

void ProfanityPatrol::scanForProfanity(const std::string& message, const std::string& channel) {
    for (const auto& profanity : _profanityList) {
        if (message.find(profanity) != std::string::npos) {
            std::string response = "PRIVMSG " + channel + " :Warning: Profanity detected!";
            sendMessage(response);
            break;
        }
    }
}

void ProfanityPatrol::sendMessage(const std::string& message) {
    std::string fullMessage = message + "\r\n";
    send(_socket, fullMessage.c_str(), fullMessage.length(), 0);
}

void ProfanityPatrol::run() {
    if (!connectToServer()) {
        return;
    }

    authenticate();

    struct pollfd pfd;
    pfd.fd = _socket;
    pfd.events = POLLIN;

    while (true) {
        int ret = poll(&pfd, 1, -1);
        if (ret > 0) {
            if (pfd.revents & POLLIN) {
                processMessages();
            }
        } else if (ret < 0) {
            std::cerr << "Poll error" << std::endl;
            break;
        }
    }
}



int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <address> <port> <password>" << std::endl;
        return 1;
    }

    std::string address = argv[1];
    int port = std::atoi(argv[2]);
    std::string password = argv[3];

    ProfanityPatrol bot(address, port, password);
    bot.run();

    return 0;
}