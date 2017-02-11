#include "client.hpp"

ClientIa::ClientIa(ClientPlayer *_player) : player(_player)
{
}

ClientIa::~ClientIa()
{
}

void	ClientIa::startPlay()
{
	map<string, int> itemsToFind;

	itemsToFind["linemate"] = 10;
	itemsToFind["phiras"] = 1;

	findItemStart(&itemsToFind, &ClientIa::endPlay);
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
	player->setInventory(inventory);
	player->printInventory();
	if (player->getInventory()[FOOD] < minFood)
	{
		itemsToFind[FOOD] = nToEat;
		findItemStart(NULL, &ClientIa::checkFoodEnd);
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

void	ClientIa::concatItemsToFind(map<string, int> *newItemsToFind)
{
	if (newItemsToFind)
	{
		for (map<string, int>::iterator it = newItemsToFind->begin() ; it != newItemsToFind->end() ; ++it)
		{
			if (itemsToFind.find(it->first) != itemsToFind.end())
				itemsToFind[it->first] += it->second;
			else
				itemsToFind[it->first] = it->second;
		}
	}
}

void	ClientIa::findItemStart(map<string, int> *newItemsToFind, void (ClientIa::*caller)())
{
	pushCallbackCallerContinue(caller);
	nRotate = 0;
	concatItemsToFind(newItemsToFind);
	if (this->itemsToFind.find(FOOD) == this->itemsToFind.end() || this->itemsToFind[FOOD] <= 0)
		checkFoodStart(MIN_FOOD_1, N_TO_EAT_1, &ClientIa::findItemSee);
	else
		findItemSee();
}

void	ClientIa::findItemSee()
{
	player->voir();
	stackCallbackCommand.push_back(&ClientIa::callbackCommandSee);
}

void	ClientIa::callbackTake(string response)
{
	if (response == SUCCESS)
	{
		itemsToFind[itemTryToTake.first]--;
		player->setCaseItemsSeen(itemTryToTake.second, itemTryToTake.first,	player->getItemsSeen()[itemTryToTake.second][itemTryToTake.first] - 1);
		cout << itemTryToTake.first << ": " << player->getItemsSeen()[itemTryToTake.second][itemTryToTake.first] << endl;
		printItemsToFind();
	}
	else
		player->setCaseItemsSeen(itemTryToTake.second, itemTryToTake.first, 0);
	findItemMove();
}

void	ClientIa::findItemMove()
{
	if (itemsToFindIsEmpty())
	{
		execCallbackCallerContinue();
		return ;
	}
	pair<string, int> itemAvailable = checkItemAvailable();
	if (itemAvailable.second == 0) /*!= -1*/
	{
		//move to case ;)
		itemTryToTake = itemAvailable;
		player->prend(itemAvailable.first);
		stackCallbackCommand.push_back(&ClientIa::callbackTake);
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
		while (nAvance > 0)
		{
			player->avance();
			stackCallbackCommand.push_back(&ClientIa::receiveCallbackCommandIgnore);
			nAvance--;
		}
		findItemStart(NULL, NULL);
	}
}

void	ClientIa::callbackCommandSee(string items)
{
	player->initItemsSeen(items);
	player->printItemsSeen();
	findItemMove();
}

void	ClientIa::receiveCallbackCommandIgnore(string response)
{
	(void)response;
}

pair<string, int>	ClientIa::checkItemAvailable()
{
	size_t						i = 0;
	vector<map<string, int> >	itemsSeen;

	itemsSeen = player->getItemsSeen();
	while (i < player->getItemsSeen().size())
	{
		for (map<string, int>::iterator item = itemsToFind.begin() ; item != itemsToFind.end() ; ++item)
		{
			if (itemsSeen[i].find(item->first) != itemsSeen[i].end() && itemsSeen[i][item->first] > 0 && itemsToFind[item->first] > 0)
				return (make_pair(item->first, i));
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

void	ClientIa::printItemsToFind()
{
	cout<<KRED<<"items to find: " << endl;
	for(map<string, int>::const_iterator it = itemsToFind.cbegin(); it != itemsToFind.cend(); ++it)
	{
		cout <<it->first << " " << it->second << endl;
	}
	cout <<KRESET;
}
