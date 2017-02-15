#include "ClientIa.hpp"

void	ClientIa::levelUpStart(void (ClientIa::*caller)())
{
	pushCallbackCallerContinue(caller);
	newClientStart(&ClientIa::callbackContinueLevelUpNewClient);
}

void	ClientIa::callbackContinueLevelUpNewClient()
{
	if (player->getLevel() == LEVEL_MAX)
	{
		execCallbackCallerContinue();
		return ();
	}
	player->inventaire();
	pushCallbackCommand(&ClientIa::callbackCommandLevelUpInventory);
}

void	ClientIa::callbackCommandLevelUpInventory(string inventory)
{
	map<string, int> itemsToFind;

	itemsToFind["linemate"] = 5;
	itemsToFind["phiras"] = 5;
	player->setInventory(inventory);
	player->printInventory();
	findItemStart(&itemsToFind, &ClientIa::callbackContinueLevelUpFindItem);
}

void	ClientIa::callbackContinueLevelUpFindItem()
{
	checkStart(MIN_FOOD_2, N_TO_EAT_2, &ClientIa::callbackContinueLevelUpCheck);
}

void	ClientIa::callbackContinueLevelUpCheck()
{
	nBroadcast++;
	if (nBroadcast % MODULO_BROADCAST == 0)
	{
		player->broadcast("Pret pour incantation level " + to_string(player->getLevel()));
	}
}

void	ClientIa::receiveCallbackBroadcast(string broadcast)
{
	(void)broadcast;
}