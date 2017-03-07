#include "clientIa.hpp"

void	ClientIa::checkNeedFoodStart(int minFood, int nToEat, void (ClientIa::*yes)(), void (ClientIa::*no)(), void (ClientIa::*end)())
{
	pushCallbackCallerContinue(yes);
	pushCallbackCallerContinue(no);
	pushCallbackCallerContinue(end);
	this->minFood = minFood;
	this->nToEat = nToEat;
	pushBackCallbackCommand(&ClientPlayer::inventaire, &ClientIa::callbackCommandcheckNeedFoodInventory, "[checkNeedFood] inventaire()");
}

void	ClientIa::callbackCommandcheckNeedFoodInventory(string inventory)
{
	void (ClientIa::*end)() = stackCallbackCallerContinue.front();
	stackCallbackCallerContinue.pop_front();
	void (ClientIa::*no)() = stackCallbackCallerContinue.front();
	stackCallbackCallerContinue.pop_front();
	void (ClientIa::*yes)() = stackCallbackCallerContinue.front();
	stackCallbackCallerContinue.pop_front();

	player->setInventory(inventory);
	if (player->getInventory()[FOOD] < minFood) //faim
	{
		itemsToFind[FOOD] = nToEat;
		(this->*yes)();
		findItemStart(NULL, end);
	}
	else
		(this->*no)();
}