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
		return ;
	}
	player->inventaire();	
	pushCallbackCommand(&ClientIa::callbackCommandLevelUpInventory);
}

void	ClientIa::callbackCommandLevelUpInventory(string inventory)
{
	map<string, int>	itemsToLevelUp;

	itemsToFind.clear(); //alegay
	player->setInventory(inventory);
	player->printInventory();

	itemsToLevelUp = player->getItemsLevelUp();
	subItemsToFind(&itemsToLevelUp);
	findItemStart(&(itemsToLevelUp), &ClientIa::callbackContinueLevelUpFindItem);
}

void	ClientIa::subItemsToFind(map<string, int> *newItemsToFind)
{
	map<string, int>	inventory;

	inventory = player->getInventory();
	if (newItemsToFind)
	{
		for (map<string, int>::iterator it = newItemsToFind->begin() ; it != newItemsToFind->end() ; ++it)
		{
			if (inventory.find(it->first) != inventory.end())
			{
				if ((*newItemsToFind)[it->first] - it->second > 0)
					(*newItemsToFind)[it->first] -= it->second;
				else
					(*newItemsToFind)[it->first] = 0;
			}
		}
	}
}

void	ClientIa::callbackContinueLevelUpFindItem()
{
	checkStart(MIN_FOOD_2, N_TO_EAT_2, &ClientIa::callbackContinueLevelUpCheck);
}

void	ClientIa::callbackContinueLevelUpCheck()
{
	nBroadcast++;
	/*
	if level == 1
		incantation()
		*/
	if (nBroadcast % MODULO_BROADCAST == 0)
	{
		player->broadcast("qui veut incanter niveau " + to_string(player->getLevel() + 1) + " " + to_string(pid));
		pushCallbackCommand(&ClientIa::callbackCommandLevelUpBroadcast);
	}
}

void	ClientIa::callbackCommandLevelUpBroadcast(string response)
{
	nPlayersBroadcastResponse = 0;
	nPlayersReadyForIncantation = 0;
	flagWaitingForIncantation = true;/*penser a le mettre false au moment opportun*/
	player->gauche();
	pushCallbackCommand(&ClientIa::receiveCallbackCommandIgnore);
	player->gauche();
	pushCallbackCommand(&ClientIa::receiveCallbackCommandIgnore);
	player->gauche();
	pushCallbackCommand(&ClientIa::receiveCallbackCommandIgnore);
	player->gauche();
	pushCallbackCommand(&ClientIa::callbackCommandLevelUpCheckBroadcastResponse);

}

void	ClientIa::callbackCommandLevelUpCheckBroadcastResponse(string response)
{
	if (nPlayersBroadcastResponse >= player->getNPlayerLevelUp() - 1)
	{
		if (nPlayersReadyForIncantation == nPlayersBroadcastResponse)
		{
			/*player->pose();
			pushCallbackCommand(&ClientIa::);
			player->incantation();
			pushCallbackCommand(&ClientIa::);*/
		}
	}
}

void	ClientIa::receiveBroadcast(string broadcast)
{
	smatch match;

	if (regex_search(broadcast, match, regex("message ([0-8]),qui veut incanter niveau ([2-7]) ([0-9]+)\n")))
	{
		if ((flagWaitingForIncantation == false && player->getLevel() == stoi(match[2]) - 1) || (flagWaitingForIncantation == true && pid > stoi(match[3])))
		{
			flagWaitingForIncantation = false;
			player->broadcast("j'arrive pour incantation level " + to_string(player->getLevel() + 1));
			pushCallbackCommand(&ClientIa::receiveCallbackCommandIgnore);
			flagBroadcastMode = true;
			flagGoToBroadcaster = true;
			stackCallbackCallerContinue.clear();
			goToBroadcaster(stoi(match[1]));
		}
		
	}
	else if (flagWaitingForIncantation && regex_search(broadcast, match, regex("message ([0-8]),j'arrive pour incantation level ([2-7])\n")))
	{
		if (player->getLevel() == stoi(match[2]) - 1)
		{
			nPlayersBroadcastResponse++;
		}
	}
	else if (flagWaitingForIncantation && regex_search(broadcast, match, regex("message ([0-8]),je suis sur zone level ([2-7])\n")))
	{
		if (player->getLevel() == stoi(match[2]) - 1 && stoi(match[1]) == 0)
		{
			nPlayersBroadcastResponse++;
			nPlayersReadyForIncantation++;
		}
	}
	else if (flagGoToBroadcaster && regex_search(broadcast, match, regex("message ([0-8]),rejoignez moi pour incantation level ([2-7])\n")))
	{
		if (player->getLevel() == stoi(match[2]) - 1)
			goToBroadcaster(stoi(match[1]));
	}
}

void	ClientIa::goToBroadcaster(int target)
{
	(void)target;
	// a faire mvoyez ???
}