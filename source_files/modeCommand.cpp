#include <Server.hpp>

// // class Server {
// // public:
// //     void modeCommand(Client* client,   ParseMessage& parsedMsg);

// // private:
// //     void handleChannelMode(Client* client,   std::string& channelName,
// 	  std::vector<std::string>& params);
// //     void handleUserMode(Client* client,   std::string& target,
// 	  std::vector<std::string>& params);
// //     void processChannelModes(Client* client, Channel& channel,
// 	  std::vector<std::string>& params);
// //     bool processSingleChannelMode(Client* client, Channel& channel,
// 	char mode, bool isAdding,
// //                                     std::vector<std::string>& params,
// 	size_t& paramIndex);
// //     bool handleKeyMode(Client* client, Channel& channel, bool isAdding,
// //                          std::vector<std::string>& params,
// 	size_t& paramIndex);
// //     bool handleLimitMode(Client* client, Channel& channel, bool isAdding,
// //                            std::vector<std::string>& params,
// 	size_t& paramIndex);
// //     bool handleOperatorMode(Client* client, Channel& channel, bool isAdding,
// //                               std::vector<std::string>& params,
// 	size_t& paramIndex);
// //     void processUserModes(Client* client,   std::string& modeString);
// //     bool isChannelInServer(  std::string& name)  ;
// //     Channel& getChannel(  std::string& name);
// // };



void Server::processChannelModes(Client *client, Channel &channel,
	  std::vector<std::string> &params)
{
	bool	isAdding;
	size_t	paramIndex;
	char	mode;

	std::string modeString = params[1];
	isAdding = true;
	paramIndex = 2;
	std::string modeChanges = ":" + client->getNickname() + " MODE "
		+ channel.getName() + " " + modeString;
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

bool Server::processSingleChannelMode(Client *client, Channel &channel,
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
		client->sendReply(ERR_UNKNOWNMODE(client->getNickname(), std::string(1,
					mode)));
		return (false);
	}
}

bool Server::handleKeyMode(Client *client, Channel &channel, bool isAdding,
	  std::vector<std::string> &params, size_t &paramIndex)
{
	if (isAdding && paramIndex < params.size())
	{
		channel.setKey(params[paramIndex++]);
		return (true);
	}
	else if (isAdding)
	{
		client->sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE +k"));
	}
	else
	{
		channel.removeKey();
	}
	return (false);
}

bool Server::handleLimitMode(Client *client, Channel &channel, bool isAdding,
	  std::vector<std::string> &params, size_t &paramIndex)
{
	int	maxUsers;

	if (isAdding && paramIndex < params.size())
	{
		maxUsers = atoi(params[paramIndex++].c_str());
		channel.setMaxUsers(maxUsers);
		return (true);
	}
	else if (isAdding)
	{
		client->sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE +l"));
	}
	else
	{
		channel.removeMaxUsers();
	}
	return (false);
}

bool Server::handleOperatorMode(Client *client, Channel &channel, bool isAdding,
	  std::vector<std::string> &params, size_t &paramIndex)
{
	if (paramIndex < params.size())
	{
		std::string targetNick = params[paramIndex++];
		if (isAdding)
			channel.addOperator(targetNick);
		else
			channel.removeOperator(targetNick);
		return (true);
	}
	else
	{
		client->sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE o"));
	}
	return (false);
}

void Server::handleUserMode(Client *client,   std::string &target,
	  std::vector<std::string> &params)
{
	if (target != client->getNickname())
	{
		client->sendReply(ERR_USERSDONTMATCH(client->getNickname()));
		return ;
	}
	if (params.size() > 1)
	{
		processUserModes(client, params[1]);
	}
	client->sendReply(RPL_UMODEIS(client->getNickname(), client->getModes()));
}

void Server::processUserModes(Client *client,   std::string &modeString)
{
	bool	isAdding;
	char	mode;

	isAdding = true;
	for (size_t i = 0; i < modeString.length(); ++i)
	{
		mode = modeString[i];
		if (mode == '+' || mode == '-')
		{
			isAdding = (mode == '+');
			continue ;
		}
		switch (mode)
		{
		case 'i':
		case 'w':
			client->setMode(mode, isAdding);
			break ;
		default:
			client->sendReply(ERR_UMODEUNKNOWNFLAG(client->getNickname()));
			break ;
		}
	}
}

static void handleChannelMode(Client *client, std::string &channelName,
	  std::vector<std::string> &params)
{
	if (!Server::isChannelInServer(channelName))
	{
		// client->sendReply(ERR_NOSUCHCHANNEL(client->getNickname(),
		// 		channelName));
		//send a reply here
		return ;
	}
	Channel &channel = Server::getChannel(channelName);
	const std::string nick = client->getNickname();
	if (!channel.isOperator(nick))
	{
		// client->sendReply(ERR_CHANOPRIVSNEEDED(client->getNickname(),
		// 		channelName));
		//
		return ;
	}
	if (params.size() > 1)
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

	if (params.empty())
	{
		// client->sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
		// the send params message
		return ;
	}

	std::string target = params[0];
	if (target[0] == '#' || target[0] == '&')
	{
		handleChannelMode(client, target, params);
	}
	else
	{
		handleUserMode(client, target, params);
	}
}