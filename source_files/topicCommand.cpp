#include <Server.hpp>

void Server::topicCommand(Client *client, const ParseMessage &ParsedMsg)
{
    std::vector<std::string> params = ParsedMsg.getParams();
    std::string response = "";
    std::string channelName;
    std::string newTopic;

    if (params.empty()) {
        client->serverReplies.push_back(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
        return;
    }
    channelName = params[0];
    if (channelName[0] != '#' && channelName[0] != '&') {
        return;
    }
    if (!isChannelInServer(channelName)) {
        response = ERR_NOSUCHCHANNEL(client->getNickname(), channelName);
        client->serverReplies.push_back(response);
        return;
    }
    Channel &channel = getChannel(channelName);
    // Check if client is in the channel
    if (!channel.isClientInChannel(client->getNickname())) {
        response = ERR_NOTONCHANNEL(client->getNickname(), channelName);
        client->serverReplies.push_back(response);
        return;
    }
    // If no topic is provided as second param or trailing message
    if (params.size() == 1 && ParsedMsg.getTrailing().empty()) {
        if (channel.getTopic().empty()) {
            response = RPL_NOTOPIC(client->getNickname(), channelName);
        } else 
		{
            response = RPL_TOPIC(client->getNickname(), channelName, channel.getTopic());
        }
        client->serverReplies.push_back(response);
        return;
    }
    if (channel.checkMode('t') && !channel.isOperator(client->getNickname())) {
        response = ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName);
        client->serverReplies.push_back(response);
        return;
    }
    newTopic = ParsedMsg.getTrailing().empty() ? params[1] : ParsedMsg.getTrailing();
    if (newTopic == channel.getTopic()) 
	{
        return;
    }
    channel.setTopic(newTopic);
    std::string topicChangeMsg = RPL_CHANGETOPIC(user_id(client->getNickname(), client->getUsername()), channel.getChannelName(), newTopic);
	channel.broadcastMessage(topicChangeMsg);
}
