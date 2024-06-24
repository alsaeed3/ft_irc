/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmususa <tmususa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:50:49 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/23 19:40:50 by tmususa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <Channel.hpp>

//CONSTRUCTOR
Channel::Channel(std::string &channelName, Client &client) : channelName(channelName), _topic(""), _key("")
{
	_topic = "";
	_key = "";
	operators[client.getNickname()] = client;
	users[client.getNickname()] = client;
	modes['i'] = false;
	modes['t'] = false;
	modes['k'] = false;
	modes['o'] = false;
	modes['l'] = false;
}

//GETTER
Channel &Server::getChannel(std::string channelName)
{
	std::map<std::string, Channel>::iterator chan = this->_channels.find(channelName);
	return chan->second;
}

bool	Server::isChannelInServer(std::string &channelName)
{
	std::map<std::string, Channel>::iterator chan = this->_channels.find(channelName);
	if(chan != this->_channels.end())
	{
		return true;
	}
	return false;
}
void Channel::addClient(Client &client)
{
	std::string nick = client.getNickname();
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


bool Channel::isClientInChannel(std::string &nickname)
{
	if(std::find(users.begin(), users.end(), nickname) != users.end())
	{
		return true;
	}

	return false;
}

bool Channel::isInInvite(std::string &nickname)
{
	if(this->inviteList.find(nickname) != inviteList.end())
	{
		return true;
	}
	return false;
}


std::map<std::string, Client> Channel::getUsers()
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
	if(std::find(operators.begin(), operators.end(), nickname) != operators.end())
	{
		return true;
	}

	return false;
}

void Channel::inviteClient(Client &client)
{
	std::string nick = client.getNickname();
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
