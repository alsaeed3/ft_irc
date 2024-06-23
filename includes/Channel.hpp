/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamzaou <shamzaou@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 17:22:28 by shamzaou          #+#    #+#             */
/*   Updated: 2024/06/23 17:23:54 by shamzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>

class Client;

class Channel {
private:
    std::string             _name;
    std::string             _topic;
    std::string             _key;
    std::map<int, Client*>  _clients;
    std::map<int, bool>     _operators;
    bool                    _inviteOnly;
    bool                    _topicRestricted;
    int                     _userLimit;

public:
    Channel(const std::string& name);
    ~Channel();

    void addClient(Client* client);
    void removeClient(Client* client);
    void broadcastMessage(const std::string& message, Client* sender);

    bool isClientInChannel(Client* client) const;
    bool isOperator(Client* client) const;

    void setTopic(const std::string& topic, Client* client);
    void setKey(const std::string& key, Client* client);
    void setInviteOnly(bool inviteOnly, Client* client);
    void setTopicRestricted(bool topicRestricted, Client* client);
    void setUserLimit(int userLimit, Client* client);

    void addOperator(Client* client);
    void removeOperator(Client* client);

    void inviteClient(Client* client, Client* invitedBy);
    void kickClient(Client* client, Client* kickedBy, const std::string& reason);

    // Getters
    const std::string& getName() const;
    const std::string& getTopic() const;
    bool isInviteOnly() const;
    bool isTopicRestricted() const;
    int getUserLimit() const;
};

#endif