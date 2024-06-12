#ifndef COMMAND_HPP
#define COMMAND_HPP

//Do a forward declaration for the other commands in the 


class Command
{
	public:
		Command();
		~Command();

		void join();
		void kick();
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