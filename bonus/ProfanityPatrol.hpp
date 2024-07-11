/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProfanityPatrol.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamzaou@student.42abudhabi.ae <shamzaou>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:49:41 by shamzaou@student  #+#    #+#             */
/*   Updated: 2024/07/10 20:20:44 by shamzaou@student ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROFANITYPATROL_HPP
#define PROFANITYPATROL_HPP

#include <string>
#include <vector>

class ProfanityPatrol {
private:
    std::string _nickname;
    std::string _username;
    std::vector<std::string> _badWords;
    int _socket;
    std::string _currentChannel;

    void loadBadWords(const std::string& filename);
    bool containsBadWord(const std::string& message) const;
    bool login(const std::string& password);
    void joinChannel(const std::string& channel);
    void sendMessage(const std::string& message);
    void kickUser(const std::string& channel, const std::string& user, const std::string& reason);

public:
    ProfanityPatrol(const std::string& nickname, const std::string& username);
    ~ProfanityPatrol();

    void start(const std::string& host, int port, const std::string& password);
    void run();
    bool processMessages();
    bool connectToServer(const std::string& host, int port);
};

#endif /* PROFANITYPATROL_HPP */