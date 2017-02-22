#include "clientIa.hpp"

void	ClientIa::checkStart(int minFood, int nToEat, void (ClientIa::*caller)())
{
	pushCallbackCallerContinue(caller);
	this->minFood = minFood;
	this->nToEat = nToEat;
	newClientStart(&ClientIa::callbackContinueCheckInventory);
}

void	ClientIa::callbackContinueCheckInventory()
{
	pushBackCallbackCommand(&clientPlayer::inventaire, &ClientIa::callbackCommandCheckInventory, "check inventaire()");
}

void	ClientIa::checkEnd()
{
	execCallbackCallerContinue();
}

void	ClientIa::callbackCommandCheckInventory(string inventory)
{
	player->setInventory(inventory);
	//player->printInventory();
	if (player->getInventory()[FOOD] < minFood)
	{
		itemsToFind[FOOD] = nToEat;
		findItemStart(NULL, &ClientIa::checkEnd);
	}
	else
		checkEnd();
}