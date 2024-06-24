/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmususa <tmususa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:48:38 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/23 19:37:13 by tmususa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <IrcLibs.hpp>
#include <IrcException.hpp>
#include <sys/socket.h>
#include <Server.hpp>

class Channel 
{
	private:
		int totalUsers;
		std::string _topic;
		std::string _key;
		std::map<std::string, Client> operators;
		std::map<std::string, Client> users;
		std::map<std::string, Client> inviteList;
		std::string &channelName;
		std::map<char, bool> modes;
		bool	_inviteOnly;
		bool	_topicRestricted;
		
		int maxUsers; 

	public:
	
		void	broadcastMessage(const std::string &message);
		Channel(std::string &channelName, Client &client);
		~Channel();

		//ADD FUNCTIONS		
		void addClient(Client &client);
		void inviteClient(Client &client);
		void addOperator(Client &client);

		//REMOVE FUNCTIONS
		void removeUser(Client &client, Client *kickedBy, const std::string &reason);
		void removeInvite(std::string &invite);
		void removeOperator(Client &client);
		
		//GETTERS
		std::string getKey( void );
		std::map<std::string, Client> getUsers();
		
		//SETTERS
		void setTopic(std::string &topic);
		void setInviteOnly(bool inviteOnly, Client* client);
		void setTopicRestricted(bool topicRestricted, Client* client);
		void setUserLimit(int userLimit, Client* client);
		void setKey(std::string &password);

		//CHECK FUNCTIONS
		bool isClientInChannel(std::string &nickname);
		bool isOperator(std::string &nickname);
		bool isInInvite(std::string &nickname);
		bool checkMode(char c);

};

#endif /* CHANNEL_HPP */