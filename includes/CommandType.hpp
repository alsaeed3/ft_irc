/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandType.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 07:45:59 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/09 12:21:39 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef COMMANDTYPE_HPP
# define COMMANDTYPE_HPP

#include <string>

enum CommandType {
	
	CMD_PASS,
	CMD_NICK,
	CMD_USER,
	CMD_JOIN,
	CMD_PRIVMSG,
	CMD_UNKNOWN
};

CommandType getCommandType( const std::string &command );

#endif /* COMMANDTYPE_HPP */