/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tofaramususa <tofaramususa@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:50:49 by alsaeed           #+#    #+#             */
/*   Updated: 2024/07/01 22:33:30 by tofaramusus      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <Channel.hpp>

//CONSTRUCTOR
Channel::Channel(std::string &channelName, Client *client) : channelName(channelName)
{
	operators[client->getNickname()] = client;
	users[client->getNickname()] = client;
	modes['i'] = false;
	modes['t'] = false;
	modes['k'] = false;
	modes['o'] = false;
	modes['l'] = false;
}

Channel::~Channel(){}

//GETTER
Channel &Server::getChannel(std::string channelName)
{
	std::map<std::string, Channel>::iterator chan = _channels.find(channelName);
	return chan->second;
}

bool	Server::isChannelInServer(std::string &channelName)
{
	std::map<std::string, Channel>::iterator chan = _channels.find(channelName);
	if(chan != _channels.end())
	{
		return true;
	}
	return false;
}
void Channel::addClient(Client *client)
{
	std::string nick = client->getNickname();
	users[nick] = client;
	if(isInInvite(nick))
	{
		inviteList.erase(nick);
	}
	if(operators.size() == 0)
	{
		operators[nick] = client;
	}
}

void Channel::setTopic(std::string &topic)
{
		_topic = topic;
}


bool Channel::isClientInChannel(std::string nickname)
{
	std::map<std::string, Client *>::iterator user_itr = this->users.find(nickname);
	if(user_itr != this->users.end())
	{
		return true;
	}
	return false;
}

bool Channel::isInInvite(std::string nickname)
{
	
	if(this->inviteList.find(nickname) != inviteList.end())
	{
		return true;
	}
	return false;
}


std::map<std::string, Client *> Channel::getUsers( void )
{
	return this->users;
}

bool Channel::checkMode(char c)
{
	if(this->modes.find(c) != this->modes.end())
	{
		return this->modes.find(c)->second;
	}
	return false;
}

bool Channel::isOperator(std::string &nickname)
{
	
	if(this->operators.find(nickname) != operators.end())
	{
		return true;
	}

	return false;
}

void Channel::inviteClient(Client *client)
{
	std::string nick = client->getNickname();
	this->inviteList[nick] = client;
}

void Channel::setKey(std::string &password)
{
	this->_key = password;
}

std::string Channel::getKey()
{
	return this->_key;
}

int Channel::getMaxUsers()
{
	return maxUsers;
}

void Channel::broadcastMessage(const std::string message)
{
    std::map<std::string, Client *>::iterator it;
    for (it = users.begin(); it != users.end(); ++it)
    {
        if (it->second->getFd() != -1)
        {
			it->second->serverReplies.push_back(message);
        }
    }
}

void Channel::sendToOthers(Client *client, std::string message)
{
    std::map<std::string, Client *>::iterator it;
    for (it = users.begin(); it != users.end(); ++it)
    {
        if (it->second->getFd() != -1 && it->second != client)
        {
            it->second->serverReplies.push_back(message);
        }
    }
}

void Channel::setMode(char c, bool setting)
{
	std::map<char, bool>::iterator itr = modes.find(c);
	if(itr != modes.end())
	{
		itr->second = setting;
	}
}

std::string Channel::getChannelName() const
{
	return channelName;
}

void Channel::removeKey()
{
	_key.clear();
}

void Channel::setMaxUsers(int limit)
{
	if(limit > 0)
		maxUsers = limit;
	else
		std::cout << "Invalid user limit inputted by the user" << std::endl;
}

void Channel::removeMaxUsers()
{
	maxUsers = -1;
}

void Channel::addOperator(std::string nickname)
{
	std::map<std::string, Client *>::iterator user_itr = this->users.find(nickname);
	
	if (user_itr != this->users.end())
	{
		this->operators[user_itr->second->getNickname()] = user_itr->second;
	}
}

void Channel::removeOperator(std::string nickname)
{
	std::map<std::string, Client*>::iterator operator_itr = this->operators.find(nickname);
    if (operator_itr != this->operators.end())
	{
        this->operators.erase(operator_itr);
    }
}