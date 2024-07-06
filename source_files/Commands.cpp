/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tofaramususa <tofaramususa@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 07:48:18 by alsaeed           #+#    #+#             */
/*   Updated: 2024/07/06 21:12:26 by tofaramusus      ###   ########.fr       */
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
    if (params[0] == "LS") {
        client->serverReplies.push_back(":irssi CAP * LS :  \r\n");
    } else if (params[0] == "REQ") {
        client->serverReplies.push_back(":irssi CAP * REQ:  \r\n");
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
	
	std::vector<std::string> params;
	if(parsedMsg.getCmd().empty() == true) //check if it is empty 
	{
		return;
	}
	printCommand(parsedMsg);
	command = parsedMsg.getCmd();
	params = parsedMsg.getParams();
	if(params.size() < 1 && parsedMsg.getTrailing().empty() == true && command != "PING" &&  command != "QUIT")
	{
		 client->serverReplies.push_back(ERR_NEEDMOREPARAMS(std::string("ircserver") ,command));
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
            client->serverReplies.push_back(ERR_ALREADYREGISTERED(client->getNickname()));
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
		else if(command == "MOTD")
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