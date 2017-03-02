#include "client.hpp"

ClientIa::ClientIa(ClientPlayer *_player) : player(_player)
{
	flagFork = false;
	pid = getpid();
	//player->init(&nextCallbackCommand);
	curCallbackCommand = NULL;
}

ClientIa::~ClientIa()
{
}

void	ClientIa::startPlay()
{
	levelUpStart(&ClientIa::endPlay);
}

void	ClientIa::endPlay()
{
	cout << "End" << endl;
}

void	ClientIa::loopCheckFood()
{
	checkStart(MIN_FOOD_1, N_TO_EAT_1, &ClientIa::loopCheckFood);
}

void	ClientIa::execCallbackCallerContinue()
{
	if (stackCallbackCallerContinue.size() > 0)
	{
		void (ClientIa::*func)() = stackCallbackCallerContinue.front();
		stackCallbackCallerContinue.pop_front();
		(this->*func)();
	}
}

void	ClientIa::callbackCommandIgnore(string response)
{
	(void)response;
}

void	ClientIa::receiveCallbackCommand(string response)
{
	if (curCallbackCommand)
	{
		if (curCallbackCommand->callback)
			(this->*curCallbackCommand->callback)(response);
		delete curCallbackCommand;
		curCallbackCommand = NULL;
	}
	else
		cout << KRED << "[ERROR] receive command not expected [" << response << "]" << KRESET << endl;
	sendNextCommand();
}

void	ClientIa::sendNextCommand()
{
	if (stackCallbackCommand.size() > 0 && curCallbackCommand == NULL)
	{
		CallbackCommand *nextCommand = stackCallbackCommand.front();
		stackCallbackCommand.pop_front();
		if (nextCommand->command)
			(player->*(nextCommand->command))(nextCommand->arg);
		curCallbackCommand = nextCommand;
	}
}

void	ClientIa::pushCallbackCallerContinue(void (ClientIa::*callbackCallerContinue)())
{
	if (callbackCallerContinue != NULL)
		stackCallbackCallerContinue.push_front(callbackCallerContinue);
}

void	ClientIa::pushBackCallbackCommand(CallbackCommand *callbackCommand)
{
	stackCallbackCommand.push_back(callbackCommand);
	sendNextCommand();
}


void	ClientIa::pushBackCallbackCommand(void (ClientPlayer::*command)(string), void (ClientIa::*callback)(string), string debug)
{
	stackCallbackCommand.push_back(new CallbackCommand(command, callback, debug));
	sendNextCommand();
}

void	ClientIa::pushFrontCallbackCommand(CallbackCommand *callbackCommand)
{
	stackCallbackCommand.push_front(callbackCommand);
	sendNextCommand();
}

void	ClientIa::callbackCommandClear()
{
	stackCallbackCommand.clear();
	if (curCallbackCommand)
		curCallbackCommand->callback = NULL;
}

/*void	ClientIa::ignoreCallbackCommand()
{
	for (auto it = stackCallbackCommand.begin() ; it != stackCallbackCommand.end() ; ++it)
	{
		*it = NULL;
	}
}*/

void	ClientIa::printStateIa(string c)
{
	ostringstream s;

	s << c;
	if (DEBUG_MODE == 0)
	{
		if (curCallbackCommand)
			s << KGRN <<" state: " << curCallbackCommand->debug << KRESET;
		s << endl;
	}
	else
	{
		s << endl << getpid() <<" stack callbackCommand:" << endl;
		if (curCallbackCommand)
			s << "\t-> " << getpid() << " " << curCallbackCommand->debug << endl;
		for(auto it = stackCallbackCommand.begin(); it != stackCallbackCommand.end() ; ++it)
		{
			s << "\t   " << getpid() << " " << (*it)->debug << endl;
		}
	}
	cout << s.str() << KRESET;
}
