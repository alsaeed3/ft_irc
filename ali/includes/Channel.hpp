/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmususa <tmususa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:48:38 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/23 17:57:31 by tmususa          ###   ########.fr       */
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
		std::string _pass;
		std::vector<std::string, Client> operators;
		std::vector<std::string, Client> users;
		std::vector<std::string, Client> inviteList;
		std::string &channelName;
		std::map<char, bool> modes;
		bool	_inviteOnly;
		bool	_topicRestricted;
		
		int maxUsers; 

	public:

		void	addClient( int cleint_fd );
		void	removeClient( int client_fd );
		void	broadcastMessage( const std::string &message, int sender_fd );
		

		Channel(std::string &channelName, Client &client);
		~Channel();
	//getters and setters
	//kickout the user function
		// void kickUser(Client &client); 

		void addUser(Client &client); //add user
		void inviteClient(Client &client);
		void addOperator(Client &client);


		void remUser(Client &client, Client *kickedBy, const std::string &reason); //rem user
		void remInvite(std::string &invite);
		void remOperator(Client &client);
		
		void setTopic(std::string &topic);
		void setInviteOnly(bool inviteOnly, Client* client);
		void setTopicRestricted(bool topicRestricted, Client* client);
		void setUserLimit(int userLimit, Client* client);

		bool isClientInChannel(std::string &nickname);
		bool isOperator(std::string &nickname);
		std::vector<std::string> getUsers();
		bool checkMode(char c);
		void setPass(std::string &password);
		std::string getPass();

		
};

#endif /* CHANNEL_HPP */