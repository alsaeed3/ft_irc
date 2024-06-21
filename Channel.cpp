#include "Includes/Channel.hpp"

Channel::Channel() : channelName(channelName), topic(""), pass("") {}

Channel::Channel(std::string &nickname, std::string &channelName) : channelName(channelName), topic(""), pass("")
{
	topic = "";
	pass = "";
	// operators.push_back(nickname);
	//users.push_back(nickname);
	// modes.push_back(make_pair('i', false));
	// modes.push_back(make_pair('t', false));
	// modes.push_back(make_pair('k', false));
	// modes.push_back(make_pair('o', false));
	// modes.push_back(make_pair('l', false));
	totalUsers = 1;


}

void Channel::addUser(User &user)
{
	//increase total users;
	//remove the user nickname from inviteList if it is there 
}


void Channel::setTopic(std::string &topic)
{

}


bool Channel::isInChannel(std::string &nickname)
{
	if(std::find(users.begin(), users.end(), nickname) != users.end())
	{
		return true;
	}

	return false;
}


std::vector<std::string> Channel::getUsers()
{
	return this->users;
}

bool Channel::checkMode(char c)
{
	if(this->modes.find(c) != this->modes.end())
	{
		return this->modes.find(c)->second;
	}
	return false;
}

bool Channel::isOperator(std::string &nickname)
{
	if(std::find(operators.begin(), operators.end(), nickname) != operators.end())
	{
		return true;
	}

	return false;
}

void Channel::addInvite(std::string &nickname)
{
	this->inviteList.push_back(nickname);
}

void Channel::setPass(std::string &password)
{
	this->pass = password;
}

std::string Channel::getPass()
{
	return this->pass;
}