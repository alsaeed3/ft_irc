#ifndef USER_HPP
#define USER_HPP

#include "Command.hpp"
//forward declaration

class User 
{
	private:
	//channel limit for number of users
		std::string username;
		std::string nickname;
		int fd; //this is the number associated with connection to server
	

	public:
	//getters and setters
		User(std::string name, int fd);
		std::string getNick();
		void setNick(std::string nick);
};



#endif