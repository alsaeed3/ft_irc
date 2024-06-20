#ifndef COMMAND_HPP
#define COMMAND_HPP

//Do a forward declaration for the other commands in the 


class Command
{
	public:
		Command();
		~Command();


		//Only operator can eject(kick, changemode or invite if in invite mode or change topic if in t channle mode)
		//JOIN command
		void join();
		//KICK command
		void kick();
		//INVITE
		void invite();
		void topic();
		void privmsg();
		void mode();
		void part();
		void notice();

		//function to split the message passed
		//channel exist
		//user exist
};



#endif