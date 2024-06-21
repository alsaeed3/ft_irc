/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 07:48:18 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/19 18:58:33 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

int	Server::registerConnection( Client* client, const ParseMessage& parsedMsg ) {

	if ( client->isRegistered() ) {

		
	}
	
	
	return 0;
}

void Server::processCommand( Client *client, const ParseMessage& parsedMsg) {

	std::string command = parsedMsg.getCommand();

	if (  )
	
	return;
}
