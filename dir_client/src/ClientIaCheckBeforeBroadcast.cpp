#include "clientIa.hpp"
/*
void	ClientIa::checkBeforeBroadcastStart(int minFood, int nToEat, void (ClientIa::*caller)())
{
	pushCallbackCallerContinue(caller);
	this->minFood = minFood;
	this->nToEat = nToEat;
	player->inventaire();
	stackCallbackCommand.push_back(&ClientIa::callbackCommandCheckBeforeBroadcastInventory);
}

void	ClientIa::callbackContinueCheckBeforeBroadcastEnd()
{
	levelUpStart(NULL);
}

void	ClientIa::callbackCommandCheckBeforeBroadcastInventory(string inventory)
{
	player->setInventory(inventory);
	if (player->getInventory()[FOOD] < minFood)
	{
		itemsToFind[FOOD] = nToEat;
		stackCallbackCallerContinue.pop_front();
		findItemStart(NULL, &ClientIa::callbackContinueCheckBeforeBroadcastEnd);
	}
	else
		execCallbackCallerContinue();
}*/