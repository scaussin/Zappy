#include "client.hpp"

ClientIa::ClientIa(ClientPlayer *_player) : player(_player)
{
	nBroadcast = 0;
}

ClientIa::~ClientIa()
{
}

void	ClientIa::startPlay()
{
	map<string, int> itemsToFind;

	itemsToFind["linemate"] = 5;
	itemsToFind["phiras"] = 5;
	itemsToFind["nourriture"] = 5;
	itemsToFind["sibur"] = 5;
	itemsToFind["mendiane"] = 5;
	itemsToFind["deraumere"] = 5;
	itemsToFind["thystame"] = 5;

	levelUpStart(&ClientIa::endPlay);
	//newClientStart(&ClientIa::newClientEnd);
	//findItemStart(&itemsToFind, &ClientIa::endPlay);
}

void	ClientIa::endPlay()
{
	cout << "End" << endl;
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

void	ClientIa::receiveCallbackCommandIgnore(string response)
{
	(void)response;
}

void	ClientIa::receiveCallbackCommand(string response)
{
	if (stackCallbackCommand.size() > 0)
	{
		void (ClientIa::*func)(string) = stackCallbackCommand.front();
		stackCallbackCommand.pop_front();
		(this->*func)(response);
	}
}

void	ClientIa::pushCallbackCallerContinue(void (ClientIa::*callbackCallerContinue)())
{
	if (callbackCallerContinue != NULL)
		stackCallbackCallerContinue.push_front(callbackCallerContinue);
}

void	ClientIa::pushCallbackCommand(void (ClientIa::*callbackCommand)(string reponse))
{
	if (callbackCommand != NULL)
		stackCallbackCommand.push_back(callbackCommand);
}

