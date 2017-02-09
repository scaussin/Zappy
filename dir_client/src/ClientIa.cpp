#include "../includes/Client.hpp"

ClientIa::ClientIa(ClientPlayer *_player) : player(_player)
{
}

ClientIa::~ClientIa()
{
}

void	ClientIa::startPlay()
{

}

void	ClientIa::callbackInventory(string inventory)
{
	cout << "inventory: " << inventory << endl;
}

void	ClientIa::checkFoodStart(int minFood, int nToEat, void (ClientIa::*callbackEnd)())
{
	this->minFood = minFood;
	this->nToEat = nToEat;
	player->inventaire();
	stackCallbackCommand.push_back(&ClientIa::callbackCheckFoodInventory);
	pushCallbackEnd(callbackEnd);
}

void	ClientIa::callbackCheckFoodInventory(string inventory)
{
	map<string, int> itemsToFind;

	player->setInventory(inventory);
	player->printInventory();
	if (player->getInventory()[FOOD] < minFood)
	{
		itemsToFind[FOOD] = nToEat;
		findItemStart(itemsToFind);
	}
	execCallbackEnd();
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
		checkFoodStart(MIN_FOOD_1, N_TO_EAT_1, &ClientIa::findItemSee);
	}
	else
		findItemSee();
}

void	ClientIa::findItemSee()
{

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
		stackCallbackEnd.push_back(callbackEnd);
}

void	ClientIa::pushCallbackCommand(void (ClientIa::*callbackCommand)(string reponse))
{
	if (callbackCommand != NULL)
		stackCallbackCommand.push_back(callbackCommand);
}