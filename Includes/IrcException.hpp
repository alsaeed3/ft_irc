/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 09:24:21 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/27 17:45:10 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef IRCEXCEPTION_HPP
# define IRCEXCEPTION_HPP

#include <IrcLibs.hpp>

class IrcException : public std::exception {

	private:

		std::string _message;

	public:

		explicit IrcException( const std::string &message ) : _message( message ) {}
		virtual ~IrcException() throw() {}
		virtual const char *what() const throw() {
			
			return _message.c_str();
		}
};

#endif /* IRCEXCEPTION_HPP */