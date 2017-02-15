#include "clientIa.hpp"

void	ClientIa::checkStart(int minFood, int nToEat, void (ClientIa::*caller)())
{
	pushCallbackCallerContinue(caller);
	newClientStart(&ClientIa::callbackContinueCheckInventory);
}

void	ClientIa::callbackContinueCheckInventory()
{
	this->minFood = minFood;
	this->nToEat = nToEat;
	player->inventaire();
	stackCallbackCommand.push_back(&ClientIa::callbackCommandCheckInventory);
}

void	ClientIa::checkEnd()
{
	execCallbackCallerContinue();
}

void	ClientIa::callbackCommandCheckInventory(string inventory)
{
	player->setInventory(inventory);
	player->printInventory();
	if (player->getInventory()[FOOD] < minFood)
	{
		itemsToFind[FOOD] = nToEat;
		findItemStart(NULL, &ClientIa::checkEnd);
	}
	else
		checkEnd();
}