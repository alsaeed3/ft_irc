/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandType.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 07:48:18 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/09 07:52:06 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CommandType.hpp"
#include <string>

CommandType getCommandType( const std::string &command ) {

	if ( command.find( "PASS " ) == 0 ) {

		return CMD_PASS;
	}

	if ( command.find( "NICK " ) == 0 ) {

		return CMD_NICK;
	}

	if ( command.find( "USER " ) == 0 ) {

		return CMD_USER;
	}

	if ( command.find( "JOIN " ) == 0 ) {

		return CMD_JOIN;
	}

	if ( command.find( "PRIVMSG " ) == 0 ) {

		return CMD_PRIVMSG;
	}

	return CMD_UNKNOWN;
}

// Path: includes/CommandType.hpp