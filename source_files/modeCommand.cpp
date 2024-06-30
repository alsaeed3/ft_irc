/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:31 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/30 16:33:11 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

// 21:24 Syntax:
// 21:24 
// 21:24 MODE <your nick>|<chanName> [<mode> [<mode parameters>]]
// 21:24 
// 21:24 Parameters:
// 21:24 
// 21:24     The target nickname or chanName and the modes with their parameters to set or
// 21:24     remove.
// 21:24 
// 21:24     If the target nickname or chanName is omitted, the active nickname or chanName
// 21:24     will be used.
// 21:24 
// 21:24 Description:
// 21:24 
// 21:24     Modifies the user or chanName modes for which you are privileged to modify.
// 21:24 
// 21:24     You can specify multiple modes in one command and prepend them by using the
// 21:24     '+' sign to set or '-' sign to unset; modes that require a parameter will be
// 21:24     retrieved from the argument list.
// 21:24 
// 21:24     Some common chanName modes are:

// 21:24         i:               When enabled, users need to be invited into the chanName
// 21:24                          or have a matching entry in the invite list.
// 21:24         k <key>:         Adds or removes a chanName key, aka a password; users
// 21:24                          will not be able to join the chanName without providing
// 21:24                          the key.
// 21:24         l <count>:       Adds, removes or updates the maximum amount of users
// 21:24         t:               When enabled, users who are not opped or voices cannot
// 21:24                          modify the chanName topic.
// 21:24         o <nickname>:    Adds or removes the operator status from a nickname..
// 21:24 
// 21:24     /MODE #irssi +o mike
// 21:24     /MODE #irssi -o+iI mike mike!*@*.irssi.org
// 21:24     /MODE +b mike!*@*.irssi.org
// 21:24     /MODE -o mike
// 21:24     /MODE +impsnkl secret_hideout 100
// 21:24     /MODE mike +iw

void Server::modeCommand(Client *client, const ParseMessage &parsedMsg)
{
	const std::vector<std::string> &params = parsedMsg.getParams();
	const std::string &command = parsedMsg.getCmd();
	std::string channelName = params[0];
	std::string mode = params[1];
	Channel *channel = findChannel(channelName);

	if ( channel == NULL )
	{
		client->serverReplies.push_back(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
		return;
	}

	if (params.size() == 2)
	{
		client->serverReplies.push_back(RPL_CHANNELMODEIS(client->getNickname(), channelName, channel->getModes()));
		return;
	}

	if (mode[0] == '+' || mode[0] == '-')
	{
		if (mode[0] == '+')
		{
			for ( std::size_t i = 1; i < mode.size(); i++ )
			{
				if (mode[i] == 'o')
				{
					if (params.size() < 3)
					{
						client->serverReplies.push_back(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
						return;
					}
					std::string nickname = params[2];
					Client *op = findClient(nickname);
					if ( op == NULL )
					{
						client->serverReplies.push_back(ERR_NOSUCHNICK(client->getNickname(), nickname));
						return;
					}
					if (channel->getOperators().find(nickname) == channel->getOperators().end())
					{
						client->serverReplies.push_back(ERR_NOTOPLEVEL(client->getNickname(), nickname));
						return;
					}
					channel->addOperator(*op);
					client->serverReplies.push_back(RPL_UMODEIS(client->getNickname(), nickname, "+o"));
				}
			}
		}
		else if (mode[0] == '-')
		{
			for ( std::size_t i = 1; i < mode.size(); i++ )
			{
				if (mode[i] == 'o')
				{
					if (params.size() < 3)
					{
						client->serverReplies.push_back(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
						return;
					}
					std::string nickname = params[2];
					Client *op = findClient(nickname);
					if ( op == NULL )
					{
						client->serverReplies.push_back(ERR_NOSUCHNICK(client->getNickname(), nickname));
						return;
					}
					if (channel->getOperators().find(nickname) == channel->getOperators().end())
					{
						client->serverReplies.push_back(ERR_NOTOPLEVEL(client->getNickname(), nickname));
						return;
					}
					channel->removeOperator(*op);
					client->serverReplies.push_back(RPL_UMODEIS(client->getNickname(), nickname, "-o"));
				}
			}
		}
	}
}
