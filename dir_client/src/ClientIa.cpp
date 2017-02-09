#include "../includes/Client.hpp"

ClientIa::ClientIa(ClientPlayer *_player) : player(_player)
{
}

ClientIa::~ClientIa()
{
}

void	ClientIa::startPlay()
{
		pushCallbackEnd(&ClientIa::endPlay);
		checkFoodStart(MIN_FOOD_1, N_TO_EAT_1);
}

void	ClientIa::endPlay()
{
		cout << "End" << endl;
}

void	ClientIa::callbackInventory(string inventory)
{
	cout << "inventory: " << inventory << endl;
}

void	ClientIa::checkFoodStart(int minFood, int nToEat)
{
	this->minFood = minFood;
	this->nToEat = nToEat;
	player->inventaire();
	stackCallbackCommand.push_back(&ClientIa::callbackCheckFoodInventory);
}

void	ClientIa::checkFoodEnd()
{
	execCallbackEnd();
}

void	ClientIa::callbackCheckFoodInventory(string inventory)
{
	map<string, int> itemsToFind;

	player->setInventory(inventory);
	player->printInventory();
	if (player->getInventory()[FOOD] < minFood)
	{
		itemsToFind[FOOD] = nToEat;
		pushCallbackEnd(&ClientIa::checkFoodEnd);
		findItemStart(itemsToFind);
	}
	checkFoodEnd();
}

void	ClientIa::execCallbackEnd()
{
	if (stackCallbackEnd.size() > 0)
	{
		(this->*stackCallbackEnd.front())();
		stackCallbackEnd.pop_front();
	}
}

void	ClientIa::findItemStart(map<string, int> itemsToFind)
{
	this->itemsToFind = itemsToFind;
	if (this->itemsToFind.find(FOOD) == this->itemsToFind.end() || this->itemsToFind[FOOD] <= 0)
	{
		pushCallbackEnd(&ClientIa::findItemSee);
		checkFoodStart(MIN_FOOD_1, N_TO_EAT_1);
	}
	else
	{
		findItemSee();
	}
}

void	ClientIa::findItemSee()
{
	player->avance();
	stackCallbackCommand.push_back(&ClientIa::callbackFindItemSee);
	cout << "findItemSee" << endl;
}

void	ClientIa::callbackFindItemSee(string items)
{
	cout << "callbackFindItemSee" << endl;
	nRotate = 0;
	if (itemsToFindIsEmpty())
	{
		execCallbackEnd();
		return ;
	}
	cout << items << endl;
	execCallbackEnd();
}

void	ClientIa::receiveCallbackBroadcast(string broadcast)
{
}

void	ClientIa::receiveCallbackCommand(string response)
{
	if (stackCallbackCommand.size() > 0)
	{
		(this->*stackCallbackCommand.front())(response);
		stackCallbackCommand.pop_front();
	}
}

void	ClientIa::pushCallbackEnd(void (ClientIa::*callbackEnd)())
{
	if (callbackEnd != NULL)
		stackCallbackEnd.push_front(callbackEnd);
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