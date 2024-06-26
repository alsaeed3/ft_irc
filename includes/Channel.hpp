/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:48:38 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/09 11:52:52 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <set>
#include <string>
#include <vector>
#include <IrcException.hpp>
#include <sys/socket.h>

class Channel {

	private:

		std::set<int>	_clients;

	public:

		void	addClient( int cleint_fd );
		void	removeClient( int client_fd );
		void	broadcastMessage( const std::string &message, int sender_fd );
};

#endif /* CHANNEL_HPP */