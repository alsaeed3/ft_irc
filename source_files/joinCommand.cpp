#include <Server.hpp>


void Server::joinCommand(Client &client, const ParseMessage &message)
{
	//need to make some changes and add some new checks for params
	std::vector<std::string> chan_list = ft_split(channels, ',');
	std::vector<std::string> key_list = ft_split(keys, ',');
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
		if (this->isChannelInServer(chanName))
		{
			Channel tempChannel = this->getChannel(chanName);
			if (tempChannel.checkMode('i')
				&& !tempChannel.isInInvite(client.getNickname()))
			{
				response = "NOT ON INVITE LIST";
				continue ;
			}
			if (tempChannel.checkMode('k'))
			{
				if (*itr_key == tempChannel.getKey())
				{
					++itr_key;
				}
				else
				{
					response = "WRONG PASSWORD";
					continue ;
				}
			}
			tempChannel.addClient(client);
		}
		else
		{
			this->_channels.insert(make_pair(chanName, Channel(chanName,
						client)));
			response = "CHANNEL CREATED USER ADDED";
		}
	}
	client.serverReplies.push_back(response);
}