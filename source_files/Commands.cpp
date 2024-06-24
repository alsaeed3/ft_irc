/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tofaramususa <tofaramususa@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 07:48:18 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/24 17:15:18 by tofaramusus      ###   ########.fr       */
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

bool Server::registerConnection(Client *client, const ParseMessage &parsedMsg) {
    const std::string &command = parsedMsg.getCmd();
    const std::vector<std::string> &params = parsedMsg.getParams();

    if (client->isRegistered() == true) {
        if (command == "USER" || command == "PASS") {
            client->serverReplies.push_back(ERR_ALREADYREGISTERED(client->getNickname()));
            return true;
        }
        return false;
    }

    if (command == "CAP") {
        handleCapCommand(client, params);
        return true;
    }

    if (command == "PASS") {
        return handlePassCommand(client, params);
    }

    if (command == "USER" && client->getIsCorrectPassword()) {
        addNewUser(client, parsedMsg);
        return true;
    }

    if (command == "NICK" && client->getIsCorrectPassword() && !client->isRegistered()) {
        nickCommand(client, params);
        // TODO: Call message of the day function
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
    if (!client->getIsCorrectPassword()) {
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
        "INVITE", "PING", "MOTD", "CAP", "PASS", "USER", 0
    };

    for (const char** cmd = validCommands; *cmd; ++cmd) {
        if (command == *cmd) 
			return true;
    }
    return false;
}

void Server::processCommand(Client *client, const ParseMessage &parsedMsg)
{
	std::string command;
	//create print command debugger message
	if(parsedMsg.getCmd().empty() == true)
	{
		return;
	}
	command = parsedMsg.getCmd();
	if(parsedMsg.getParams().size() < 1 && parsedMsg.getTrailing().empty() == true && command != "PING")
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
			// joinCommand();
		}
		if(command == "NICK")
		{
			nickCommand(client, parsedMsg.getParams());
		}

		return;
	}
	if(registerConnection(client, parsedMsg) == false)
	{
		//send message that says register user first
	}
}