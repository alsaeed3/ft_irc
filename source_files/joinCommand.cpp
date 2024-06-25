#include <Server.hpp>


void Server::joinCommand(Client *client, const ParseMessage &ParsedMsg)
{
	//need to make some changes and add some new checks for params

	std::cout << "here" << std::endl;
	std::vector<std::string> params = ParsedMsg.getParams();
	std::vector<std::string> key_list;
	if(params.size() < 1)
	{
		client->serverReplies.push_back(ERR_NEEDMOREPARAMS(client->getNickname(), "JOIN"));
	}
	std::vector<std::string> chan_list = ft_split(params[0], ',');
	if(params.size() > 1)
	{
		 key_list = ft_split(params[1], ',');
	}
	std::vector<std::string>::iterator itr_key;
	std::vector<std::string>::iterator itr_chan;
	std::string response;

	for (itr_chan = chan_list.begin(); itr_chan != chan_list.end(); ++itr_chan)
	{
		std::string chanName = *itr_chan;
		if (chanName.at(0) != '#' && chanName.at(0) != '&')
		{
			response = "ERR_BADCHANMASK";
			continue ;
		}
		itr_key = key_list.begin();
		if (isChannelInServer(chanName))
		{
			Channel tempChannel = getChannel(chanName);
			if (tempChannel.checkMode('i')
				&& !tempChannel.isInInvite(client->getNickname()))
			{
				response = "NOT ON INVITE LIST";
				continue ;
			}
			if (tempChannel.checkMode('k'))
			{
				if (itr_key != key_list.end() && *itr_key == tempChannel.getKey())
				{
					++itr_key;
				}
				else
				{
					response = "WRONG_PASSWORD";
					continue ;
				}
			}
			tempChannel.addClient(*client);
		}
		else
		{
			_channels.insert(make_pair(chanName, Channel(chanName,
						*client)));
			response = "CHANNEL CREATED USER ADDED";
		}
	}
	client->serverReplies.push_back(response);
}