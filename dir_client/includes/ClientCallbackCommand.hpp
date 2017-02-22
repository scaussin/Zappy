#ifndef CALLBACKCOMMAND_HPP
# define CALLBACKCOMMAND_HPP

# include "client.hpp"


class CallbackCommand
{
	public:
		CallbackCommand(void (ClientPlayer::*command)(string), void (ClientIa::*callback)(string), string arg, string debug);
		CallbackCommand(void (ClientPlayer::*command)(string), void (ClientIa::*callback)(string), string debug);
		~CallbackCommand();
		
		void						(ClientPlayer::*command)(string);
		void						(ClientIa::*callback)(string);
		string						arg;
		string						debug;
};

#endif