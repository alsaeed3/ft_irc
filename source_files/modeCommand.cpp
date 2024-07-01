#include <Server.hpp>

static bool handleKeyMode(Client *client, Channel &channel, bool isAdding,
	  std::vector<std::string> &params, size_t &paramIndex)
{
	if (isAdding && paramIndex < params.size())
	{
		channel.setKey(params[paramIndex++]);
		return (true);
	}
	else if (isAdding)
	{
		client->serverReplies.push_back(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE +k"));
	}
	else
	{
		channel.removeKey();
	}
	return (false);
}

static bool handleLimitMode(Client *client, Channel &channel, bool isAdding,
	  std::vector<std::string> &params, size_t &paramIndex)
{
	int	maxUsers;

	if (isAdding && paramIndex < params.size())
	{
		maxUsers = std::atoi(params[paramIndex++].c_str());
		channel.setMaxUsers(maxUsers);
		return (true);
	}
	else if (isAdding)
	{
		client->serverReplies.push_back(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE +l"));
	}
	else
	{
		channel.removeMaxUsers();
	}
	return (false);
}

static bool handleOperatorMode(Client *client, Channel &channel, bool isAdding,
	  std::vector<std::string> &params, size_t &paramIndex)
{
	if (paramIndex < params.size())
	{
		std::string targetNick = params[paramIndex++];
		if(!channel.isClientInChannel(targetNick))
		{
			client->serverReplies.push_back(ERR_USERNOTINCHANNEL(client->getNickname(), targetNick, channel.getChannelName()));
		}
		if (isAdding)
			channel.addOperator(targetNick);
		else
			channel.removeOperator(targetNick);
		return (true);
	}
	else
	{
		client->serverReplies.push_back(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE o"));
	}
	return (false);
}

// void Server::handleUserMode(Client *client,   std::string &target,
// 	  std::vector<std::string> &params)
// {
// 	if (target != client->getNickname())
// 	{
// 		client->sendReply(ERR_USERSDONTMATCH(client->getNickname()));
// 		return ;
// 	}
// 	if (params.size() > 1)
// 	{
// 		processUserModes(client, params[1]);
// 	}
// 	client->sendReply(RPL_UMODEIS(client->getNickname(), client->getModes()));
// }

static bool processSingleChannelMode(Client *client, Channel &channel,
	char mode, bool isAdding,   std::vector<std::string> &params,
	size_t &paramIndex)
{
	switch (mode)
	{
	case 'i':
		channel.setMode('i', isAdding);
		return (false);
	case 'k':
		return (handleKeyMode(client, channel, isAdding, params, paramIndex));
	case 'l':
		return (handleLimitMode(client, channel, isAdding, params, paramIndex));
	case 't':
		channel.setMode('t', isAdding);
		return (false);
	case 'o':
		return (handleOperatorMode(client, channel, isAdding, params,
				paramIndex));
	default:
		client->serverReplies.push_back(ERR_UNKNOWNMODE(client->getNickname(), std::string(1,
					mode)));
		return (false);
	}
}

// static void Server::processUserModes(Client *client,   std::string &modeString)
// {
// 	bool	isAdding;
// 	char	mode;

// 	isAdding = true;
// 	for (size_t i = 0; i < modeString.length(); ++i)
// 	{
// 		mode = modeString[i];
// 		if (mode == '+' || mode == '-')
// 		{
// 			isAdding = (mode == '+');
// 			continue ;
// 		}
// 		switch (mode)
// 		{
// 		case 'i':
// 		case 'w':
// 			client->setMode(mode, isAdding);
// 			break ;
// 		default:
// 			client->serverReplies.push_back(ERR_UMODEUNKNOWNFLAG(client->getNickname()));
// 			break ;
// 		}
// 	}
// }

void processChannelModes(Client *client, Channel &channel,
	  std::vector<std::string> &params)
{
	bool	isAdding;
	size_t	paramIndex;
	char	mode;

	std::string modeString = params[0];
	isAdding = true;
	paramIndex = 1;
	std::string modeChanges = ":" + client->getNickname() + " MODE "
		+ channel.getChannelName() + " " + modeString;
	for (size_t i = 0; i < modeString.length(); ++i)
	{
		mode = modeString[i];
		if (mode == '+' || mode == '-')
		{
			isAdding = (mode == '+');
			continue ;
		}
		if (processSingleChannelMode(client, channel, mode, isAdding, params,
				paramIndex))
		{
			modeChanges += " " + params[paramIndex - 1];
		}
	}
	channel.broadcastMessage(modeChanges);
}

static void handleChannelMode(Client *client, std::string &channelName,
	  std::vector<std::string> &params)
{
	if (!Server::isChannelInServer(channelName))
	{
		client->serverReplies.push_back(ERR_NOSUCHCHANNEL(client->getNickname(),
				channelName));
		return ;
	}
	Channel &channel = Server::getChannel(channelName);
	const std::string nick = client->getNickname();
	if (!channel.isOperator(const_cast<std::string &>(client->getNickname())))
	{
		client->serverReplies.push_back(ERR_CHANOPRIVSNEEDED(client->getNickname(),
				channelName));
		return ;
	}
	if (params.size() > 0)
	{
		processChannelModes(client, channel, params);
	}
	else
	{
		// client->sendReply(RPL_CHANNELMODEIS(client->getNickname(), channelName,
		// 		channel.getModes()));
		//send message that has the channel modes
	}
}

void Server::modeCommand(Client *client, const ParseMessage &parsedMsg)
{
	  std::vector<std::string> params = parsedMsg.getParams();

	  //l need to check the mode string for the letters that they are just  +- etc.
	  //l need to add the trailing to the params also;
	  //l also need to check that the par


	if (params.empty())
	{
		client->serverReplies.push_back(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
		return ;
	}
	std::string target = params[0];
	params.erase(params.begin());
	if (target[0] == '#' || target[0] == '&')
	{
		handleChannelMode(client, target, params);
	}
	else
	{
		client->serverReplies.push_back(ERR_NOSUCHCHANNEL(client->getNickname(), target));
	}
}