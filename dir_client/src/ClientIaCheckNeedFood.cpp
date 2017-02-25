#include "clientIa.hpp"

void	ClientIa::checkNeedFoodStart(int minFood, int nToEat, void (ClientIa::*yes)(), void (ClientIa::*no)())
{
	pushCallbackCallerContinue(yes);
	pushCallbackCallerContinue(no);
	this->minFood = minFood;
	this->nToEat = nToEat;
	pushBackCallbackCommand(&ClientPlayer::inventaire, &ClientIa::callbackCommandcheckNeedFoodInventory, "[checkNeedFood] inventaire()");
}

void	ClientIa::callbackCommandcheckNeedFoodInventory(string inventory)
{
	player->setInventory(inventory);
	if (player->getInventory()[FOOD] < minFood) //faim
	{
		itemsToFind[FOOD] = nToEat;
		execCallbackCallerContinue();
		findItemStart(NULL, &ClientIa::loopCheckFood);
	}
	else
	{
		stackCallbackCallerContinue.pop_front();
		execCallbackCallerContinue();
	}
}

void	ClientIa::loopCheckFood()
{
	checkFood(MIN_FOOD_1, N_TO_EAT_1, &ClientIa::loopCheckFood);
}