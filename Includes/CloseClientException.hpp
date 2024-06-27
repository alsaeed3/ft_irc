/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CloseClientException.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:34:16 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/27 17:44:56 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLOSECLIENTEXCEPTION_HPP
# define CLOSECLIENTEXCEPTION_HPP

#include <IrcLibs.hpp>

class CloseClientException : public std::exception {

	public:

		explicit CloseClientException() {}
		virtual ~CloseClientException() throw() {}
};

#endif /* CLOSECLIENTEXCEPTION_HPP */