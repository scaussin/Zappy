#include "client.hpp"

ClientIa::ClientIa(ClientPlayer *_player) : player(_player)
{
}

ClientIa::~ClientIa()
{
}

void	ClientIa::startPlay()
{
		checkFoodStart(MIN_FOOD_1, N_TO_EAT_1, &ClientIa::endPlay);
}

void	ClientIa::endPlay()
{
	cout << "End" << endl;
}

void	ClientIa::checkFoodStart(int minFood, int nToEat, void (ClientIa::*caller)())
{
	pushCallbackCallerContinue(caller);
	this->minFood = minFood;
	this->nToEat = nToEat;
	player->inventaire();
	stackCallbackCommand.push_back(&ClientIa::callbackCheckFoodInventory);
}

void	ClientIa::checkFoodEnd()
{
	execCallbackCallerContinue();
}

void	ClientIa::callbackCheckFoodInventory(string inventory)
{
	map<string, int> itemsToFind;

	player->setInventory(inventory);
	player->printInventory();
	if (player->getInventory()[FOOD] < minFood)
	{
		itemsToFind[FOOD] = nToEat;
		findItemStart(itemsToFind, &ClientIa::checkFoodEnd);
	}
	else
		checkFoodEnd();
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

void	ClientIa::findItemStart(map<string, int> itemsToFind, void (ClientIa::*caller)())
{
	pushCallbackCallerContinue(caller);
	nRotate = 0;
	this->itemsToFind = itemsToFind;
	if (this->itemsToFind.find(FOOD) == this->itemsToFind.end() || this->itemsToFind[FOOD] <= 0)
		checkFoodStart(MIN_FOOD_1, N_TO_EAT_1, &ClientIa::findItemSee);
	else
		findItemSee();
}

void	ClientIa::findItemSee()
{
	player->voir();
	stackCallbackCommand.push_back(&ClientIa::callbackFindItemSee);
}

void	ClientIa::callbackTake(string response)
{
	/*if (ok)
		itemsToFind[itemAvailable.first]--;*/
}

void	ClientIa::callbackFindItemSee(string items)
{
	player->setItemsSeen(items);
	player->printItemsSeen();
	if (itemsToFindIsEmpty())
	{
		execCallbackCallerContinue();
		return ;
	}
	pair<string, int> itemAvailable = checkItemAvailable();
	if (itemAvailable.second == 0) /*!= -1*/
	{
		//move to case ;)
		player->prendre(itemAvailable.first);
		stackCallbackCommand.push_back(&ClientIa::);
	}
	else if (nRotate < 3)
	{
		player->droite();
		stackCallbackCommand.push_back(&ClientIa::receiveCallbackCommandIgnore);
		nRotate++;
		findItemSee();
	}
	else
	{
		int nRand = rand() % 3;
		if (nRand == 0)
		{
			player->droite();
			stackCallbackCommand.push_back(&ClientIa::receiveCallbackCommandIgnore);
		}
		else if (nRand == 1)
		{
			player->gauche();
			stackCallbackCommand.push_back(&ClientIa::receiveCallbackCommandIgnore);
		}
		int nAvance = (player->getHeightVision() * 2) - 1;
		while (nAvance >= 0)
		{
			player->avance();
			stackCallbackCommand.push_back(&ClientIa::receiveCallbackCommandIgnore);
			nAvance--;
		}
		findItemStart(itemsToFind, NULL);
	}
}

void	ClientIa::receiveCallbackCommandIgnore(string response)
{
	(void)response;
}

pair<string, int>	ClientIa::checkItemAvailable()
{
	size_t						i = 0;
	vector<map<string, int> >	itemsSeen = player->getItemsSeen();

	while (i < player->getItemsSeen().size())
	{
		for (map<string, int>::iterator item = itemsToFind.begin() ; item != itemsToFind.end() ; ++item)
		{
			if (itemsSeen[i].find(item->first) != itemsSeen[i].end())
			{
				return (make_pair(item->first, i));
			}
		}
		i++;
	}
	return make_pair("", -1);
}

void	ClientIa::receiveCallbackBroadcast(string broadcast)
{
	(void)broadcast;
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

bool	ClientIa::itemsToFindIsEmpty()
{
	for (map<string, int>::iterator it = itemsToFind.begin(); it != itemsToFind.end(); ++it)
	{
		if (it->second > 0)
			return false;
	}
	return true;
}
