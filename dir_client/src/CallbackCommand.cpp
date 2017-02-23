#include "client.hpp"

CallbackCommand::CallbackCommand(void (ClientPlayer::*command)(string), void (ClientIa::*callback)(string), string arg, string debug)
{
	this->command = command;
	this->callback = callback;
	this->arg = arg;
	this->debug = debug;
}

CallbackCommand::CallbackCommand(void (ClientPlayer::*command)(string), void (ClientIa::*callback)(string), string debug)
{
	this->command = command;
	this->callback = callback;
	this->arg = "";
	this->debug = debug;
}

CallbackCommand::~CallbackCommand()
{
}