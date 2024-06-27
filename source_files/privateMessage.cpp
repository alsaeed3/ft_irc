#include <Channel.hpp>
#include <Server.hpp>

void Server::privateMessage(Client &client, const ParseMessage &ParsedMsg)
{
	std::vector<std::string> params = ParsedMsg.getParams();
	std::string trailing = ParsedMsg.getTrailing();

	if(params.size() == 1)
	{
		std::string receiver = params[0];
		if(trailing.empty() == false)
		{
			if(receiver[0] == '#')
			{
				if(isChannelInServer(receiver))
				{
					if(getChannel(receiver).isClientInChannel(client->getNickname()))
					{
						getChannel(receiver).sendToOthers(client, RPL_PRIVMSG(client->getNickname(), client->getUsername(), receiver, trailing));
					}
				}
				else
				{
					client->serverReplies.push_back(ERR_CANNOTSENDTOCHAN(client->getNickname(), receiver));
				}
			}
			else
			{
				if(isUserInServer(receiver))
				{
					getClient(receiver)->serverReplies.push_back(RPL_PRIVMSG(client->getNickname(), client->getUsername(), receiver, trailing));
				}
				else
                	client->serverReplies.push_back(ERR_NOSUCHNICK(client->getNickname(), receiver));
			}
		}
		else
		{
			client->serverReplies.push_back(ERR_NORECIPIENT(client->getNickname()));
		}
	}
	else
		client->serverReplies.push_back(ERR_NOTEXTTOSEND(client->getNickname()));
}