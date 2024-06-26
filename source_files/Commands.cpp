/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 07:48:18 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/26 21:44:43 by alsaeed          ###   ########.fr       */
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
        if (params.size() > 2 && params[1] != "0" && params[2] != "*" && !parsedMsg.getTrailing().empty())
        {
            client->setUsername(parsedMsg.getTrailing());
        }
        else if (params.size() > 1)
        {
            client->setUsername(params[1]);
        }
        std::cout << "User registered: " << client->getUsername() 
                  << " (Real name: " << client->getUsername() << ")" << std::endl;
		//call the message of the day
    }
}

bool Server::connectUser(Client *client, const ParseMessage &parsedMsg) 
{
    const std::string &command = parsedMsg.getCmd();
    const std::vector<std::string> &params = parsedMsg.getParams();

    if (client->isRegistered() == true) 
	{
        if (command == "USER" || command == "PASS")
		 {
            client->serverReplies.push_back(ERR_ALREADYREGISTERED(client->getNickname()));
            return true;
        }
		else
        	return false;
    }

    if (command == "CAP")
	{
        handleCapCommand(client, params);
        return true;
    }

    if (command == "PASS") 
	{
        return handlePassCommand(client, params);
    }

    if (command == "USER" && client->getIsCorrectPassword()) {
        addNewUser(client, parsedMsg);
        return true;
    }

    if (command == "NICK" && client->getIsCorrectPassword() && !client->isRegistered()) 
	{
        if(nickCommand(client, params))
			client->serverReplies.push_back( RPL_MOTDSTART(client->getUsername(),client->getUsername()));
        return true;
    }
    return false;
}

// Helper functions

void Server::handleCapCommand(Client *client, const std::vector<std::string> &params) 
{
    if (params[0] == "LS") {
        client->serverReplies.push_back(":irssi CAP * LS :  \r\n");
    } else if (params[0] == "REQ") {
        client->serverReplies.push_back(":irssi CAP * REQ:  \r\n");
    }
}

bool Server::handlePassCommand(Client *client, const std::vector<std::string> &params) {
    if (client->getIsCorrectPassword() == false) 
	{
        if (params[0] == getServerPassword())
		{
            client->setIsCorrectPassword(true);
        } else 
		{
            client->serverReplies.push_back(ERR_PASSWDMISMATCH(client->getNickname()));
            closeClient(client->getFd());
            client->setFd(-1);
        }
    } else {
        client->serverReplies.push_back(ERR_ALREADYREGISTERED(client->getNickname()));
    }
    return true;
}

bool Server::isValidIRCCommand(const std::string& command) 
{
    static const char* validCommands[] = {
        "JOIN", "MODE", "TOPIC", "NICK", "QUIT", "PRIVMSG",
        "INVITE", "PING", "MOTD", "CAP", "PASS", "USER", "PART", "NOTICE", 0
    };

    for (const char** cmd = validCommands; *cmd; ++cmd) {
        if (command == *cmd) 
			return true;
    }
    return false;
}

void Server::printCommand(ParseMessage message)
{
	std::vector<std::string> params = message.getParams();
    std::cout << "Command: " << message.getCmd() << std::endl;
    for (std::size_t i = 0; i < params.size(); i++) 
	{
        std::cout << "Param " << i << ": " << params[i] << std::endl;
    }
    std::cout << "Trailing: " << message.getTrailing() << std::endl;
}

void Server::processCommand(Client *client, const ParseMessage &parsedMsg)
{
	std::string command;
	//create print command debugger message
	
	std::vector<std::string> params;
	if(parsedMsg.getCmd().empty() == true)
	{
		return;
	}
	printCommand(parsedMsg);
	command = parsedMsg.getCmd();
	params = parsedMsg.getParams();
	if(params.size() < 1 && parsedMsg.getTrailing().empty() == true && command != "PING")
	{
		 client->serverReplies.push_back(ERR_NEEDMOREPARAMS(client->getUsername() ,command));
	}
	if(isValidIRCCommand(parsedMsg.getCmd()) == false) //this will check the type of command
	{
		client->serverReplies.push_back(ERR_UNKNOWNCOMMAND(client->getNickname(), parsedMsg.getCmd()));
		return;
	}
	if (command == "QUIT")
		quitCommand(parsedMsg.getTrailing(), client);
	if (client->isRegistered() == true)
	{
		if (command == "JOIN")
		{
			joinCommand(client, parsedMsg);
		}
		if(command == "PRIVMSG")
		{
			privateMessage(client, parsedMsg);	
		}
		if(command == "PING")
		{
			 client->serverReplies.push_back(RPL_PONG(user_id(client->getNickname(),client->getUsername()),params[0]));
		}
		if(command == "NICK")
		{
			nickCommand(client, params);
		}
		if (command == "MODE")
		{
			
		}
		if(command == "TOPIC")
		{
			
		}
		if(command == "INVITE")
		{
			
		}
		if(command == "MOTD")
		{
			
		}
		if(command == "PART")
		{
			
		}
		if(command == "NOTICE")
		{
			
		}
		return;
	}
	if(connectUser(client, parsedMsg) == false)
	{
		//send message that says register user first
	}
	command.clear();
	params.clear();
}