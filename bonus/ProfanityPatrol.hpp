/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProfanityPatrol.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamzaou@student.42abudhabi.ae <shamzaou>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:49:44 by shamzaou@student  #+#    #+#             */
/*   Updated: 2024/07/10 17:56:00 by shamzaou@student ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <cctype>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <algorithm>

class ProfanityPatrol {
private:
    std::string _nickname;
    std::string _username;
    std::string _currentChannel;
    std::vector<std::string> _badWords;
    int _socket;

    // Helper functions
    void loadBadWords(const std::string& filename);
    bool containsBadWord(const std::string& message);
    void processMessage(const std::string& message);
    
    // IRC protocol functions
    void connectToServer(const std::string& host, int port);
    void login(const std::string& password);
    void joinChannel(const std::string& channel);
    void sendMessage(const std::string& message);
    void kickUser(const std::string& user, const std::string& reason);

public:
    ProfanityPatrol(const std::string& nickname, const std::string& username);
    ~ProfanityPatrol();

    void start(const std::string& host, int port, const std::string& password);
    void run();
};

