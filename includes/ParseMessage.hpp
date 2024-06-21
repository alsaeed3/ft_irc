/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseMessage.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 18:20:06 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/20 19:38:20 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef PARSEMESG_HPP

#include <IrcLibs.hpp>
#include <IrcException.hpp>

class ParseMessage {

	private:

		int 						_msgLen;
		std::string					_msg;
		std::string					_command;
		std::vector<std::string>	_params;
		std::vector<std::string>	_tags;
		std::string					_trailing;
		std::string					_prefix;
		bool						_notValidParam;
		std::string					_errorMsg;

	public:

		ParseMessage( const std::string &message );

		void 						displayCommand(  const ParseMessage &parsedMessage ) const;
		bool						isValid( const std::string &param ) const;
		std::string					ft_trim( const std::string &str ) const;

		void						setMsgLen( int msgLen ) { _msgLen = msgLen; }
		void 						setMsg( const std::string &msg ) { _msg = msg; }
		void 						setCommand( const std::string &command ) { _command = command; }
		void 						setParams( const std::vector<std::string> &params ) { _params = params; }
		void 						setTrailing( const std::string &trailing ) { _trailing = trailing; }
		void 						setPrefix( const std::string &prefix ) { _prefix = prefix; }

		int							getMsgLen( void ) const { return _msgLen; }
		std::string					getCommand( void ) const { return _command; }
		std::vector<std::string>	getParams( void ) const { return _params; }
		std::string					getTrailing( void ) const { return _trailing; }
		std::string					getPrefix( void ) const { return _prefix; }
};

#endif /* PARSEMSG_HPP */