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
	pushBackCallbackCommand(&ClientPlayer::inventaire, &ClientIa::callbackCommandLevelUpInventory, "[levelUp] inventaire()");
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
	stackCallbackCommand.clear();
	levelUpStart(&ClientIa::endPlay);
}

void	ClientIa::pushFrontElevationEnd()
{
	pushFrontCallbackCommand(new CallbackCommand(NULL, &ClientIa::callbackCommandLevelUpIncantationEnd, "[levelUp] end incantation"));
}

void	ClientIa::callbackCommandLevelUpIncantationStart(string response)
{
	cout << "incantation: " << response << endl;
	if (response == "ko\n")
		levelUpStart(NULL);
	else
		pushBackCallbackCommand(NULL, &ClientIa::callbackCommandLevelUpIncantationEnd, "[levelUp] incantationEnd()");
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
			pushBackCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandLevelUpBroadcast, 
			"qui veut incanter niveau " + to_string(player->getLevel() + 1) + " " + to_string(pid), "[levelUp] broadcast(qui veut incanter ?)"));
		}
		else
		{
			pushBackCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandLevelUpRepeatBroadcast, 
			"rejoignez moi pour incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(rejoignez moi)"));
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
	pushBackCallbackCommand(&ClientPlayer::gauche, &ClientIa::callbackCommandIgnore, "[levelUp] gauche() waiting responses incantation");
	pushBackCallbackCommand(&ClientPlayer::gauche, &ClientIa::callbackCommandIgnore, "[levelUp] gauche() waiting responses incantation");
	pushBackCallbackCommand(&ClientPlayer::gauche, &ClientIa::callbackCommandIgnore, "[levelUp] gauche() waiting responses incantation");
	pushBackCallbackCommand(&ClientPlayer::gauche, &ClientIa::callbackCommandIgnore, "[levelUp] gauche() waiting responses incantation");
	pushBackCallbackCommand(&ClientPlayer::gauche, &ClientIa::callbackCommandIgnore, "[levelUp] gauche() waiting responses incantation");
	pushBackCallbackCommand(&ClientPlayer::gauche, &ClientIa::callbackCommandIgnore, "[levelUp] gauche() waiting responses incantation");
	pushBackCallbackCommand(&ClientPlayer::gauche, &ClientIa::callbackCommandLevelUpCheckBroadcastResponse, "[levelUp] gauche() check if incantation is possible");
}

void	ClientIa::callbackContinueLevelUpIncantation()
{
	flagIsIncantationCaller = true;
	pushBackCallbackCommand(&ClientPlayer::incantation, &ClientIa::callbackCommandLevelUpIncantationStart, "[levelUp] incantationStart()");
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
		pushBackCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore,
				"abandon de l'incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(abandon)"));
		if (flagFork == false)
		{
			pushBackCallbackCommand(&ClientPlayer::fork, &ClientIa::callbackCommandIgnore, "[levelUp] fork() lay egg");
			flagFork = true;
		}
		callbackContinueLevelUpFindItem();
	}
}

void	ClientIa::callbackContinueLevelUpComming()
{
	flagGoToBroadcaster = true;
}

void	ClientIa::receiveBroadcast(string broadcast)
{
	smatch match;

	if (regex_search(broadcast, match, regex("message ([0-8]),qui veut incanter niveau ([2-8]) ([0-9]+)\n")))
	{
		if ((flagWaitingForIncantation == false && player->getLevel() == stoi(match[2]) - 1 && stoi(match[1]) != 0) || (flagWaitingForIncantation == true && pid > stoi(match[3]) && player->getLevel() == stoi(match[2]) - 1))
		{
			flagWaitingForIncantation = false;
			if (itemsToFind.find(FOOD) == itemsToFind.end() || itemsToFind[FOOD] == 0) // pas faim
			{
				stackCallbackCommand.clear();
				stackCallbackCallerContinue.clear();
				itemsToFind.clear();
				pushFrontCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore,
					"j'arrive pour incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(j'arrive)"));
				checkStart(MIN_FOOD_2, N_TO_EAT_2, &ClientIa::callbackContinueLevelUpComming);
				//goToBroadcaster(stoi(match[1]));
			}
			else
			{	
				int food = itemsToFind[FOOD];
				itemsToFind.clear();
				itemsToFind[FOOD] = food;
				pushFrontCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore,
					"j'arrive pour incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(j'arrive apres mangé)"));
			}
		}
		else if (flagWaitingForIncantation == false && player->getLevel() == stoi(match[2]) - 1 && stoi(match[1]) == 0)
		{
			/*if (itemsToFind.find(FOOD) == itemsToFind.end() || itemsToFind[FOOD] == 0) // pas faim
			{
				stackCallbackCommand.clear();
				stackCallbackCallerContinue.clear();
				itemsToFind.clear();
				pushFrontCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore,
					"j'arrive pour incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(j'arrive)"));
				checkStart(MIN_FOOD_2, N_TO_EAT_2, &ClientIa::callbackContinueLevelUpComming);
			}*/
			flagGoToBroadcaster = false;
			stackCallbackCommand.clear();
			stackCallbackCallerContinue.clear();
			itemsToFind.clear();
			//checkStart(MIN_FOOD_2, N_TO_EAT_2, &ClientIa::callbackContinueLevelUpComming);
			pushBackCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore,
				"je suis sur zone level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(je suis sur zone)"));
		}
	}
	else if (flagWaitingForIncantation == false && regex_search(broadcast, match, regex("message ([0-8]),abandon de l'incantation level ([2-8])\n")) && player->getLevel() == stoi(match[2]) - 1)
	{
		flagGoToBroadcaster = false;
		stackCallbackCommand.clear();
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
		pushBackCallbackCommand(&ClientPlayer::avance, &ClientIa::callbackCommandIgnore, "[levelUp] avance() go to master");
	if (target == 3 || target == 4)
		pushBackCallbackCommand(&ClientPlayer::gauche, &ClientIa::callbackCommandIgnore, "[levelUp] gauche() go to master");
	if (target == 7 || target == 6 || target == 5)
		pushBackCallbackCommand(&ClientPlayer::droite, &ClientIa::callbackCommandIgnore, "[levelUp] droite() go to master");
}