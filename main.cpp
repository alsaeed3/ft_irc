/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:55:22 by alsaeed           #+#    #+#             */
/*   Updated: 2024/07/08 18:57:47 by alsaeed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <Server.hpp>
#include <ProfanityPatrol.hpp>
#include <iostream>
#include <pthread.h>


void* runBot(void* arg) {
    ProfanityPatrol* bot = static_cast<ProfanityPatrol*>(arg);
    bot->run();
    return NULL;
}

int main(int ac, char* av[]) {
    // Ensure correct number of arguments
    if (ac != 3) {
        std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return -1;
    }

    // Convert port argument to integer and validate it
    int portNum = std::atoi(av[1]);
    std::string port(av[1]), password(av[2]);

    if (portNum < 1024 || portNum > 65535 || port.empty() || port.length() > 5 
        || port.find_first_not_of("0123456789") != std::string::npos) {
        std::cerr << "Invalid port number" << std::endl;
        return -2;
    }

    // Validate password is not empty
    if (password.empty()) {
        std::cerr << "Server's password is not given" << std::endl;
        return -3;
    }
    
 	Server *server = NULL;
    ProfanityPatrol *bot = NULL;
    pthread_t botThread;

    try {
        server = Server::getInstance();
        server->setServerPassword(password);
        server->setServerPort(portNum);
        server->initServer();

        // Create the bot
        bot = new ProfanityPatrol("ProfanityPatrol", "ProfanityPatrol");
        
        // Start the bot in a new thread
        if (pthread_create(&botThread, NULL, runBot, bot) != 0) {
            throw IrcException("Failed to create bot thread");
        }

        // Run the server in the main thread
        server->runServer();

        // Wait for the bot thread to finish
        pthread_join(botThread, NULL);
    } catch (const IrcException &e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        delete server;
        delete bot;
        return -4;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        delete server;
        delete bot;
        return -5;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        delete server;
        delete bot;
        return -6;
    }

    delete server;
    delete bot;

    return 0;
}