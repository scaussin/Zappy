#include "ClientIa.hpp"

void	ClientIa::levelUpStart(void (ClientIa::*caller)())
{
	nBroadcast = 0;
	flagGoToBroadcaster = false;
	flagWaitingForIncantation = false;
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
	pushBackCallbackCommand(&clientPlayer::inventaire, &ClientIa::callbackCommandLevelUpInventory, "levelUp inventaire()");
}

void	ClientIa::callbackCommandLevelUpInventory(string inventory)
{
	map<string, int>	itemsToLevelUp;

	itemsToFind.clear(); //alegay
	player->setInventory(inventory);
	//player->printInventory();

	itemsToLevelUp = player->getItemsLevelUp();
	subItemsToFind(&itemsToLevelUp);
	player->printMap(itemsToLevelUp);
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
				if ((*newItemsToFind)[it->first] - inventory[it->first] > 0)
					(*newItemsToFind)[it->first] -= inventory[it->first];
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

void	ClientIa::callbackCommandLevelUpIncantationEnd(string response)
{
	cout << "incantation end: " << response << endl;
	flagIsIncantationCaller = false;
	player->setLevel(player->getLevel() + 1);
	ignoreCallbackCommand();
	levelUpStart(NULL);
}

void	ClientIa::pushFrontElevationEnd()
{
	pushFrontCallbackCommand(&ClientIa::callbackCommandLevelUpIncantationEnd);
}

void	ClientIa::callbackCommandLevelUpIncantationStart(string response)
{
	cout << "incantation: " << response << endl;
	if (response == "ko\n")
		levelUpStart(NULL);
	else
		pushCallbackCommand(&ClientIa::callbackCommandLevelUpIncantationEnd);
}

void	ClientIa::callbackContinueLevelUpCheck()
{
	if (player->getLevel() == 1)
	{
		itemsPoseStart(player->getItemsLevelUp(), &ClientIa::callbackContinueLevelUpIncantation);
	}
	else
	{
		if (nBroadcast % MODULO_BROADCAST == 0)
		{
			flagWaitingForIncantation = true;
			player->broadcast("qui veut incanter niveau " + to_string(player->getLevel() + 1) + " " + to_string(pid));
			pushCallbackCommand(&ClientIa::callbackCommandLevelUpBroadcast);
		}
		else
		{
			player->broadcast("rejoignez moi pour incantation level " + to_string(player->getLevel() + 1));
			pushCallbackCommand(&ClientIa::callbackCommandLevelUpRepeatBroadcast);
		}
		nBroadcast++;
	}
}

void	ClientIa::callbackCommandLevelUpRepeatBroadcast(string response)
{
	(void)response;
	callbackContinueLevelUpCheck();
}

void	ClientIa::callbackCommandLevelUpBroadcast(string response)
{
	(void)response;
	nPlayersBroadcastResponse = 0;
	nPlayersReadyForIncantation = 0;
	player->gauche();
	pushCallbackCommand(&ClientIa::callbackCommandIgnore);
	player->gauche();
	pushCallbackCommand(&ClientIa::callbackCommandIgnore);
	player->gauche();
	pushCallbackCommand(&ClientIa::callbackCommandIgnore);
	player->gauche();
	pushCallbackCommand(&ClientIa::callbackCommandIgnore);
	player->gauche();
	pushCallbackCommand(&ClientIa::callbackCommandIgnore);
	player->gauche();
	pushCallbackCommand(&ClientIa::callbackCommandIgnore);
	player->gauche();
	pushCallbackCommand(&ClientIa::callbackCommandLevelUpCheckBroadcastResponse);
}

void	ClientIa::callbackContinueLevelUpIncantation()
{
	player->incantation();
	flagIsIncantationCaller = true;
	pushCallbackCommand(&ClientIa::callbackCommandLevelUpIncantationStart);
}

void	ClientIa::callbackCommandLevelUpCheckBroadcastResponse(string response)
{
	(void)response;
	if (nPlayersBroadcastResponse >= player->getNPlayerLevelUp() - 1)
	{
		if (nPlayersReadyForIncantation == nPlayersBroadcastResponse)
		{
			itemsPoseStart(player->getItemsLevelUp(), &ClientIa::callbackContinueLevelUpIncantation);
			/*cout << getpid() << "peut lancer nouvelle incantation" << endl;
			pushCallbackCommand(&ClientIa::);
			player->incantation();
			pushCallbackCommand(&ClientIa::);*/
		}
		else
		{
			callbackContinueLevelUpCheck();
		}
	}
	else
	{
		nBroadcast = 0;
		flagWaitingForIncantation = false;
		player->broadcast("abandon de l'incantation level " + to_string(player->getLevel() + 1));
		pushCallbackCommand(&ClientIa::callbackCommandIgnore);
		if (flagFork == false)
		{
			/*player->fork();
			pushCallbackCommand(&ClientIa::callbackCommandIgnore);
			flagFork = true;*/
		}
		callbackContinueLevelUpFindItem();
	}
}
/*
void	ClientIa::callbackContinueLevelUpBroadcastComing()
{
	
}
*/
void	ClientIa::receiveBroadcast(string broadcast)
{
	smatch match;

	if (regex_search(broadcast, match, regex("message ([0-8]),qui veut incanter niveau ([2-8]) ([0-9]+)\n")))
	{
		if ((flagWaitingForIncantation == false && player->getLevel() == stoi(match[2]) - 1 && stoi(match[1]) != 0) || (flagWaitingForIncantation == true && pid > stoi(match[3]) && player->getLevel() == stoi(match[2]) - 1))
		{
			if (itemsToFind.find(FOOD) == itemsToFind.end() || itemsToFind[FOOD] == 0)
			{
				flagWaitingForIncantation = false;
				flagGoToBroadcaster = true;
				player->broadcast("j'arrive pour incantation level " + to_string(player->getLevel() + 1));
				pushCallbackCommand(&ClientIa::callbackCommandIgnore);
				ignoreCallbackCommand();
				stackCallbackCallerContinue.clear();
				itemsToFind.clear();
				//goToBroadcaster(stoi(match[1]));
			}
		}
		else if (flagWaitingForIncantation == false && player->getLevel() == stoi(match[2]) - 1 && stoi(match[1]) == 0)
		{
			flagGoToBroadcaster = false;
			ignoreCallbackCommand();
			stackCallbackCallerContinue.clear();
			itemsToFind.clear();
			player->broadcast("je suis sur zone level " + to_string(player->getLevel() + 1));
			pushCallbackCommand(&ClientIa::callbackCommandIgnore);
		}
	}
	else if (flagWaitingForIncantation == false && regex_search(broadcast, match, regex("message ([0-8]),abandon de l'incantation level ([2-8])\n")) && player->getLevel() == stoi(match[2]) - 1)
	{
		flagGoToBroadcaster = false;
		ignoreCallbackCommand();
		stackCallbackCallerContinue.clear();
		itemsToFind.clear();
		levelUpStart(NULL);
	}
	else if (flagWaitingForIncantation && regex_search(broadcast, match, regex("message ([0-8]),j'arrive pour incantation level ([2-8])\n")))
	{
		if (player->getLevel() == stoi(match[2]) - 1)
		{
			nPlayersBroadcastResponse++;
		}
	}
	else if (flagWaitingForIncantation && regex_search(broadcast, match, regex("message ([0-8]),je suis sur zone level ([2-8])\n")))
	{
		if (player->getLevel() == stoi(match[2]) - 1 && stoi(match[1]) == 0)
		{
			nPlayersBroadcastResponse++;
			nPlayersReadyForIncantation++;
		}
	}
	else if (flagGoToBroadcaster && regex_search(broadcast, match, regex("message ([0-8]),rejoignez moi pour incantation level ([2-8])\n")))
	{
		if (player->getLevel() == stoi(match[2]) - 1)
			goToBroadcaster(stoi(match[1]));
	}
}

void	ClientIa::goToBroadcaster(int target)
{
	if (target == 1 || target == 2 || target == 8)
		player->avance();
	if (target == 3)
		player->gauche();
	if (target == 7)
		player->droite();
	if (target == 4)
		player->gauche();
	if (target == 6)
		player->droite();
	if (target == 5)
		player->droite();
	if (target != 0)
		pushCallbackCommand(&ClientIa::callbackCommandIgnore);
}