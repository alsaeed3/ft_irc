/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tofaramususa <tofaramususa@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:50:49 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/26 17:48:04 by tofaramusus      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <Channel.hpp>

//CONSTRUCTOR
Channel::Channel(std::string &channelName, Client &client) : channelName(channelName), _topic(""), _key("")
{
	operators[client.getNickname()] = client;
	users[client.getNickname()] = client;
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


bool Channel::isClientInChannel(std::string nickname)
{
	std::map<std::string, Client>::iterator user_itr = this->users.find(nickname);
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


std::map<std::string, Client> Channel::getUsers( void )
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

int Channel::getMaxUsers()
{
	return maxUsers;
}

void Channel::broadcastMessage(const std::string message)
{
    std::map<std::string, Client>::iterator it;
    for (it = users.begin(); it != users.end(); ++it)
    {
        int fd = it->second.getFd();
        if (fd != -1)
        {
            send(fd, message.c_str(), message.length(), 0);
        }
    }
}

void Channel::sendToOthers(Client *client, std::string message)
{
    std::map<std::string, Client>::iterator it;
    for (it = users.begin(); it != users.end(); ++it)
    {
		int fd = it->second.getFd();
        if (fd != -1 && &it->second != client)
        {
            send(fd, message.c_str(), message.length(), 0);
        }
    }
}