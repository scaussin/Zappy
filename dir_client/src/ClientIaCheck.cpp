#include "clientIa.hpp"

void	ClientIa::checkStart(int minFood, int nToEat, void (ClientIa::*caller)())
{
	pushCallbackCallerContinue(caller);
	this->minFood = minFood;
	this->nToEat = nToEat;
	player->inventaire();
	stackCallbackCommand.push_back(&ClientIa::callbackcheckInventory);
}

void	ClientIa::checkEnd()
{
	execCallbackCallerContinue();
}

void	ClientIa::callbackcheckInventory(string inventory)
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