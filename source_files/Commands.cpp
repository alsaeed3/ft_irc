/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tofaramususa <tofaramususa@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 07:48:18 by alsaeed           #+#    #+#             */
/*   Updated: 2024/07/12 16:38:12 by tofaramusus      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Channel.hpp>
#include <Server.hpp>

void Server::addNewUser(Client* client, const ParseMessage &parsedMsg)
{
	std::vector<std::string> params = parsedMsg.getParams();
	
    if (client->getUsername().empty() == true && !params.empty())
    {
        client->setUsername(params[0]);
        std::cout << "User registered: " << client->getUsername() 
                  << " (Real name: " << client->getUsername() << ")" << std::endl;
    }
}

void Server::connectUser(Client *client, const ParseMessage &parsedMsg) 
{
    const std::string &command = parsedMsg.getCmd();
    const std::vector<std::string> &params = parsedMsg.getParams();

    if (command == "CAP")
	{
        handleCapCommand(client, params);
    }
    else if (command == "PASS") 
	{
        handlePassCommand(client, params);
    }
	else if (command == "USER" && client->getIsCorrectPassword())
	{
		addNewUser(client, parsedMsg);
	}
	else if (command == "NICK" && client->getIsCorrectPassword()) 
	{
		nickCommand(client, params);
	}
	if (client->isRegistered() == true)
	{
		motdCommand(client);
	}
    return ;
}

void Server::handleCapCommand(Client *client, const std::vector<std::string> &params) 
{
    if (params.size() == 2 && params[0] == "LS" && params[1] == "302") {
		client->conRegi[0] = true;
        client->serverReplies.push_back(":irssi CAP * LS :  \r\n");
    } else if (params[0] == "REQ") {
        client->serverReplies.push_back(":irssi CAP * REQ:  \r\n");
    } else if (client->conRegi[4] == true && params[0] == "") {
		c
	}
}

bool Server::handlePassCommand(Client *client, const std::vector<std::string> &params) {
    if (client->getIsCorrectPassword() == false) 
	{
        if (!params.empty() && params[0] == getServerPassword())
		{
            client->setIsCorrectPassword(true);
        } else 
		{
            client->serverReplies.push_back(ERR_PASSWDMISMATCH(std::string("ircserver")));
            // closeClient(client->getFd());
            // client->setFd(-1);
			//ADD message to say try again password
			throw(std::exception());
        }
    } else {
        client->serverReplies.push_back(ERR_ALREADYREGISTERED(std::string("ircserver")));
    }
    return true;
}

bool Server::isValidIRCCommand(const std::string& command) 
{
    static const char* validCommands[] = {
        "JOIN", "MODE", "TOPIC", "NICK", "QUIT", "PRIVMSG", "KICK",
        "INVITE", "PING", "motd", "CAP", "PASS", "USER", "PART", "WHO", "NOTICE", "WHOIS", 0
    };

    for (const char** cmd = validCommands; *cmd; ++cmd) {
        if (command == *cmd) 
			return true;
    }
    return false;
}

void	Server::displayCommand(  const ParseMessage &parsedMessage ) const {
	std::cout << "Command: " << parsedMessage.getCmd() << std::endl;
	std::cout << "Params: ";
	for ( int i = 0; i < static_cast<int>(parsedMessage.getParams().size()); i++ ) {
		
		std::cout << "Parameter " << "[" << i << "]: " << parsedMessage.getParams()[i] << std::endl;
	}
	std::cout << std::endl;
	if(!parsedMessage.getTrailing().empty())
		std::cout << "Trailing: " << parsedMessage.getTrailing() << std::endl;

	return;
}

void Server::processCommand(Client *client, const ParseMessage &parsedMsg)
{
	std::string command;
	
	std::vector<std::string> params;
	if(parsedMsg.getCmd().empty() == true) //check if it is empty 
	{
		return;
	}
	displayCommand(parsedMsg);
	command = parsedMsg.getCmd();
	params = parsedMsg.getParams();
	if(params.size() < 1 && parsedMsg.getTrailing().empty() == true && command != "PING" &&  command != "QUIT" && command != "motd")
	{
		 client->serverReplies.push_back(ERR_NEEDMOREPARAMS(std::string("ircserver") ,command));
		 return;
	}
	if(isValidIRCCommand(parsedMsg.getCmd()) == false)
	{
		client->serverReplies.push_back(ERR_UNKNOWNCOMMAND(std::string("ircserver"), parsedMsg.getCmd()));
		return;
	}
	if (command == "QUIT")
		quitCommand(parsedMsg.getTrailing(), client);
	if(client->isRegistered() == false)
	{
		connectUser(client, parsedMsg);	
	}
	else
	{    
		if (command == "USER" || command == "PASS")
		{
            client->serverReplies.push_back(ERR_ALREADYREGISTERED(std::string("ircserver")));
        }
		else if (command == "JOIN")
		{
			joinCommand(client, parsedMsg);
		}
		else if(command == "PRIVMSG")
		{
			privateMessage(client, parsedMsg);	
		}
		else if(command == "PING")
		{
			 client->serverReplies.push_back(RPL_PONG(user_id(client->getNickname(),client->getUsername()),params[0]));
		}
		else if(command == "NICK")
		{
			nickCommand(client, params);
		}
		else if (command == "MODE")
		{
			modeCommand(client, parsedMsg);
		}
		else if(command == "TOPIC")
		{
			topicCommand(client, parsedMsg);
		}
		else if(command == "KICK")
		{
			kickCommand(client, parsedMsg);
		}
		else if(command == "INVITE")
		{
			inviteCommand(client, parsedMsg);
		}
		else if(command == "motd")
		{
			motdCommand(client);
		}
		else if(command == "PART")
		{
			partCommand(client, parsedMsg);
		}
		else if(command == "NOTICE")
		{
			noticeCommand(client, parsedMsg);
		}
	}
	return;
}