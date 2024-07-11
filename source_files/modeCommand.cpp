#include <Server.hpp>

bool Server::handleKeyMode(Client *client, Channel &channel, bool isAdding,
	  std::vector<std::string> &params, std::size_t &paramIndex)
{
	if (isAdding && paramIndex < params.size()) {

		channel.setKey(params[paramIndex++]);
		client->serverReplies.push_back(RPL_CHANNELMODEISWITHKEY(client->getNickname(), channel.getChannelName(), channel.getModes(), channel.getKey()));

		return (true);
	} else if (isAdding) {

		client->serverReplies.push_back(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE +k"));
	} else {

		channel.removeKey();
	}

	return (false);
}

bool Server::handleLimitMode(Client *client, Channel &channel, bool isAdding,
	  std::vector<std::string> &params, std::size_t &paramIndex)
{
	int	UserLimit;

	if (isAdding && paramIndex < params.size())
	{
		UserLimit = std::atoi(params[paramIndex++].c_str());
		channel.setUserLimit(UserLimit);
		return (true);
	}
	else if (isAdding)
	{
		client->serverReplies.push_back(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE +l"));
	}
	else
	{
		channel.removeUserLimit();
	}
	return (false);
}

bool Server::handleOperatorMode(Client *client, Channel &channel, bool isAdding,
	  std::vector<std::string> &params, std::size_t &paramIndex)
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

bool Server::processSingleChannelMode(Client *client, Channel &channel,
	char mode, bool isAdding,   std::vector<std::string> &params,
	std::size_t &paramIndex)
{
	switch (mode)
	{
	case 'i':
		channel.setMode('i', isAdding);
		return (true);
	case 'k':
		return (handleKeyMode(client, channel, isAdding, params, paramIndex));
	case 'l':
		return (handleLimitMode(client, channel, isAdding, params, paramIndex));
	case 't':
		channel.setMode('t', isAdding);
		return (true);
	case 'o':
		return (handleOperatorMode(client, channel, isAdding, params,
				paramIndex));
	default:
		client->serverReplies.push_back(ERR_UNKNOWNMODE(client->getNickname(), std::string(1,
					mode)));
		return (false);
	}
}

void Server::processChannelModes(Client *client, Channel &channel,
	  std::vector<std::string> &params)
{
	bool	isAdding;
	std::size_t	paramIndex;
	char	mode;

	std::string modeString = params[1];
	isAdding = true;
	paramIndex = 2;
	// std::string modeChanges = ":" + client->getNickname() + " MODE "
	// 	+ channel.getChannelName() + " " + modeString;
	std::string modeChanges = MODE_CHANNELCHANGEMODE(user_id(client->getNickname(), client->getUsername()), channel.getChannelName(), modeString);
	for (std::size_t i = 0; i < modeString.length(); ++i)
	{
		mode = modeString[i];
		if (mode == '+' || mode == '-')
		{
			isAdding = (mode == '+');
			continue ;
		}
		processSingleChannelMode(client, channel, mode, isAdding, params, paramIndex);
		// {
		// 	modeChanges += " " + params[paramIndex - 1];
		// }
	}
	channel.broadcastMessage(modeChanges);
}

void Server::handleChannelMode(Client *client, std::string &channelName,
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
	if (params.size() == 1)
	{
		client->serverReplies.push_back(RPL_CHANNELMODEIS(client->getNickname(), channelName,
				channel.getModes()));
	}
	else
	{
		if (!channel.isOperator(const_cast<std::string &>(client->getNickname())))
		{
			client->serverReplies.push_back(ERR_CHANOPRIVSNEEDED(client->getNickname(),
					channelName));
			return ;
		}
		processChannelModes(client, channel, params);
	}
}

void Server::modeCommand(Client *client, const ParseMessage &parsedMsg)
{
	  std::vector<std::string> params = parsedMsg.getParams();

	if(parsedMsg.getTrailing().empty() == false)
	{
		std::vector<std::string> splitTrailing = ft_split(parsedMsg.getTrailing(), ' ');
		params.insert(params.end(), splitTrailing.begin(), splitTrailing.end());
	}

	if (params.size() < 1)
	{
		client->serverReplies.push_back(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
		return ;
	}
	std::string target = params[0];
	// params.erase(params.begin());
	if (target[0] == '#' || target[0] == '&')
	{
		handleChannelMode(client, target, params);
	}
	else
	{
		client->serverReplies.push_back(ERR_NOSUCHCHANNEL(client->getNickname(), target));
	}
}