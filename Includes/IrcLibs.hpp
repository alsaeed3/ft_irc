/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcLibs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 18:21:12 by alsaeed           #+#    #+#             */
/*   Updated: 2024/06/19 17:57:28 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cerrno>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <netinet/in.h>
#include <stdlib.h>
#include <signal.h>
#include <cstring>
#include <set>
#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <IrcException.hpp>
#include <Client.hpp>
#include <Channel.hpp>