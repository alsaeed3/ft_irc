/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamzaou <shamzaou@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 17:24:24 by shamzaou          #+#    #+#             */
/*   Updated: 2024/06/23 17:41:33 by shamzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"

Channel::Channel(const std::string& name)
    : _name(name), _topic(""), _key(""), _inviteOnly(false),
      _topicRestricted(false), _userLimit(0) {}

Channel::~Channel() {
    // Cleanup code
}

void Channel::addClient(Client* client) {
    _clients[client->getFd()] = client;
}

void Channel::removeClient(Client* client) {
    _clients.erase(client->getFd());
    _operators.erase(client->getFd());
}

void Channel::broadcastMessage(const std::string& message, Client* sender) {
    for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second != sender) {
            it->second->sendMessage(message);
        }
    }
}

bool Channel::isClientInChannel(Client* client) const {
    return _clients.find(client->getFd()) != _clients.end();
}

bool Channel::isOperator(Client* client) const {
    std::map<int, bool>::const_iterator it = _operators.find(client->getFd());
    return it != _operators.end() && it->second;
}

void Channel::setTopic(const std::string& topic, Client* client) {
    if (!_topicRestricted || isOperator(client)) {
        _topic = topic;
    }
}

void Channel::addOperator(Client* client) {
    _operators[client->getFd()] = true;
}

void Channel::removeOperator(Client* client) {
    _operators.erase(client->getFd());
}

void Channel::kickClient(Client* client, Client* kickedBy, const std::string& reason) {
    if (isOperator(kickedBy)) {
        removeClient(client);
        client->sendMessage("You have been kicked from " + _name + " by " + kickedBy->getNickname() + " (" + reason + ")");
    }
}

void Channel::setKey(const std::string& key, Client* client) {
    if (isOperator(client)) {
        _key = key;
    }
}

void Channel::setInviteOnly(bool inviteOnly, Client* client) {
    if (isOperator(client)) {
        _inviteOnly = inviteOnly;
    }
}

void Channel::setTopicRestricted(bool topicRestricted, Client* client) {
    if (isOperator(client)) {
        _topicRestricted = topicRestricted;
    }
}

void Channel::setUserLimit(int userLimit, Client* client) {
    if (isOperator(client)) {
        _userLimit = userLimit;
    }
}

void Channel::inviteClient(Client* client, Client* invitedBy) {
    if (_inviteOnly && isOperator(invitedBy)) {
        addClient(client);
        client->sendMessage("You have been invited to " + _name + " by " + invitedBy->getNickname());
    }
}

const std::string& Channel::getName() const {
    return _name;
}

const std::string& Channel::getTopic() const {
    return _topic;
}

bool Channel::isInviteOnly() const {
    return _inviteOnly;
}

bool Channel::isTopicRestricted() const {
    return _topicRestricted;
}

int Channel::getUserLimit() const {
    return _userLimit;
}