/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseMessage.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 18:55:07 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/21 16:03:54 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ParseMessage.hpp>

std::string	ParseMessage::ft_trim( const std::string &str ) const {

	size_t start = str.find_first_not_of( " \n\r\t" );
	size_t end = str.find_last_not_of( " \n\r\t" );

	if ( start == std::string::npos || end == std::string::npos ) {

		return "";
	}

	return str.substr( start, end - start + 1 );
}

ParseMessage::ParseMessage( const std::string& message ) {

	if ( message.empty() ) {

		return;
	}

	_msg = message;
	_msgLen = static_cast<int>(message.length());
	_cmd = "";
	_params.clear();
	_trailing = "";
	_notValidParam = false;
	_errorMsg = "";
	std::string	trimmedMsg = ft_trim( message );
	std::string token;
	std::istringstream iss( trimmedMsg );
	bool tagFlag = false;
	bool tagCmd = true;

	if ( trimmedMsg[0] == '@' ) {

		tagFlag = true;
	}
	
	while ( iss >> token ) {

		if ( tagFlag ) {

			if ( token[0] == ':' ) {

				_cmd = token.substr( 1 );
				tagFlag = false;
				tagCmd = false;
				continue;
			}

			continue;
		}
		
		if ( tagCmd ) {
			
			_cmd = token;
			tagCmd = false;
			continue;
		}

		if ( token[0] == ':' ) {
			
			_trailing = ft_trim( message.substr( message.find( token ) ) );
			break;
		} else {
	
			if ( isValid( token ) ) {
				
				_params.push_back( token );
			} else {
				
				_notValidParam = true;
				_errorMsg = "Invalid character in parameter: " + token;
				break;
			}	
		}
	}

	return;
}

bool	ParseMessage::isValid( const std::string &param ) const {

	std::string invalidChars = "\n\r\t:";
	/* if find_first_of() find any invalid character in param
	returns the position of it, o/w returns std::string::npos */
	std::size_t valid = param.find_first_of( invalidChars ); 

	return valid == std::string::npos;
}

void ParseMessage::displayCommand(  const ParseMessage &parsedMessage ) const {

	std::cout << "Command: " << parsedMessage.getCommand() << std::endl;
	std::cout << "Params: ";
	for ( int i = 0; i < static_cast<int>(parsedMessage._params.size()); i++ ) {
		
		std::cout << "Parameter " << "[" << i << "]: " << parsedMessage._params[i] << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Trailing: " << parsedMessage._trailing << std::endl;
	std::cout << "Prefix: " << parsedMessage._prefix << std::endl;

	return;
}