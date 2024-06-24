/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 07:48:18 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/24 13:12:46 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Channel.hpp>
#include <Server.hpp>

// int	Server::registerConnection( Client* client, const ParseMessage& parsedMsg ) {

// 	if ( client->isRegistered() == true ) {

// 		client->serverReplies.push_back( ERR_ALREADYREGISTRED);
// 		return (0);
// 	}

// 	return (0);
// }

bool Server::UserRegi(Client *client, const ParseMessage &parsedMsg)
{
	std::string command = parsedMsg.getCmd();
	if (client->isRegistered() == true)
	{
		if (command == "USER" || command == "PASS")
			client->serverReplies.push_back(ERR_ALREADYREGISTERED(client->getNickname()));
	}
	else
	{
		if (command == "CAP")
		{
		}
	}
}

void Server::processCommand(Client *client, const ParseMessage &parsedMsg)
{
	std::string command = parsedMsg.getCmd();
	if (command == "QUIT")
		quitCmd(parsedMsg.getTrailing(), client);
	if (userRegi(client, parsedMsg) == false)
	{
	}
	else
	{
		if (command == "JOIN")
		{
			// joinCommand();
		}
		else if (command == "KICK")
		{
		}
		else if (command == "INVITE")
		{
		}
		else if (command == "PING")
		{
		}
		else if (command == "TOPIC")
		{
		}
		else if (command == "MODE")
		{
		}
		else if (command == "MOTD")
		{
		}
	}
	if (command == "NICK")
		return ;
}

void Server::joinCommand(Client &client, const ParseMessage &message)
{
	std::vector<std::string> chan_list = ft_split(channels, ',');
	std::vector<std::string> key_list = ft_split(keys, ',');
	std::vector<std::string>::iterator itr_key;
	std::vector<std::string>::iterator itr_chan;
	std::string response;

	for (itr_chan = chan_list.begin(); itr_chan != chan_list.end(); ++itr_chan)
	{
		std::string chanName = *itr_chan;
		if (chanName.at(0) != '#' && chanName.at(0) != '&')
		{
			response = "ERR_BADCHANMASK";
			continue ;
		}
		itr_key = key_list.begin();
		if (this->isChannelInServer(chanName))
		{
			Channel tempChannel = this->getChannel(chanName);
			if (tempChannel.checkMode('i')
				&& !tempChannel.isInInvite(client.getNickname()))
			{
				response = "NOT ON INVITE LIST";
				continue ;
			}
			if (tempChannel.checkMode('k'))
			{
				if (*itr_key == tempChannel.getKey())
				{
					++itr_key;
				}
				else
				{
					response = "WRONG PASSWORD";
					continue ;
				}
			}
			tempChannel.addClient(client);
		}
		else
		{
			this->_channels.insert(make_pair(chanName, Channel(chanName,
						client)));
			response = "CHANNEL CREATED USER ADDED";
		}
	}
	client.serverReplies.push_back(response);
}