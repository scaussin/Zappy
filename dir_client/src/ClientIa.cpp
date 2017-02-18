#include "client.hpp"

ClientIa::ClientIa(ClientPlayer *_player) : player(_player)
{
	nBroadcast = 0;
	flagWaitingForIncantation = false;
	flagGoToBroadcaster = false;
	nCommandIgnore = 0;
	flagIsIncantationCaller = false;
	pid = getpid();
}

ClientIa::~ClientIa()
{
}

void	ClientIa::startPlay()
{
	/*map<string, int> itemsToFind;

	itemsToFind["linemate"] = 5;
	itemsToFind["phiras"] = 5;
	itemsToFind["nourriture"] = 5;
	itemsToFind["sibur"] = 5;
	itemsToFind["mendiane"] = 5;
	itemsToFind["deraumere"] = 5;
	itemsToFind["thystame"] = 5;*/

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

void	ClientIa::callbackCommandIgnore(string response)
{
	(void)response;
}

void	ClientIa::receiveCallbackCommand(string response)
{
	//printStack(KMAG);
	if (stackCallbackCommand.size() > 0)
	{
		void (ClientIa::*func)(string) = stackCallbackCommand.front();
		stackCallbackCommand.pop_front();
		if (nCommandIgnore == 0)
			(this->*func)(response);
		else
			nCommandIgnore--;
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

void	ClientIa::pushFrontCallbackCommand(void (ClientIa::*callbackCommand)(string reponse))
{
	if (callbackCommand != NULL)
		stackCallbackCommand.push_front(callbackCommand);
}

void	ClientIa::printStack(string c)
{
	bool falg = false;
	vector<pair<void (ClientIa::*)(string reponse), string> > tab;
	ostringstream s;

	tab.push_back(make_pair(&ClientIa::callbackCommandCheckInventory, "callbackCommandCheckInventory"));
	tab.push_back(make_pair(&ClientIa::callbackCommandFindItemTake, "callbackCommandFindItemTake"));
	tab.push_back(make_pair(&ClientIa::callbackCommandFindItemSee, "callbackCommandFindItemSee"));
	tab.push_back(make_pair(&ClientIa::callbackCommandIgnore, "callbackCommandIgnore"));
	tab.push_back(make_pair(&ClientIa::callbackCommandLevelUpBroadcast, "callbackCommandLevelUpBroadcast"));
	tab.push_back(make_pair(&ClientIa::callbackCommandConnectNbr, "callbackCommandConnectNbr"));
	tab.push_back(make_pair(&ClientIa::callbackCommandLevelUpInventory, "callbackCommandLevelUpInventory"));
	tab.push_back(make_pair(&ClientIa::callbackCommandLevelUpCheckBroadcastResponse, "callbackCommandLevelUpCheckBroadcastResponse"));
	tab.push_back(make_pair(&ClientIa::callbackCommandLevelUpIncantationStart, "callbackCommandLevelUpIncantationStart"));
	tab.push_back(make_pair(&ClientIa::callbackCommandLevelUpIncantationEnd, "callbackCommandLevelUpIncantationEnd"));
	tab.push_back(make_pair(&ClientIa::callbackCommandLevelUpRepeatBroadcast, "callbackCommandLevelUpRepeatBroadcast"));

	cout <<c<< getpid() <<" stack callbackCommand:" << endl;
	for(auto it = stackCallbackCommand.begin(); it != stackCallbackCommand.end() ; ++it)
	{
		falg = false;
		for (auto itTab = tab.begin() ; itTab != tab.end(); ++itTab)
		{
			if (itTab->first == *it)
			{
				falg = true;
				s << "\t"<<getpid()<<" " << itTab->second << endl;
			}
		}
		if (!falg)
			s << "\t" << "ERROR" << endl;
	}
	cout << s.str() << KRESET;
}

