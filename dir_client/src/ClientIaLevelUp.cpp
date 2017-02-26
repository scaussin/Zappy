#include "ClientIa.hpp"

void	ClientIa::levelUpStart(void (ClientIa::*caller)())
{
	state = none;
	flagGoToBroadcaster = false;
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
	state = none;
	cout << "incantation end: " << response << endl;
	//flagIsIncantationCaller = false;
	player->setLevel(player->getLevel() + 1);
	callbackCommandClear();
	levelUpStart(&ClientIa::endPlay);
}

void	ClientIa::pushFrontElevationEnd()
{
	CallbackCommand *tmp = curCallbackCommand;
	curCallbackCommand = new CallbackCommand(NULL, &ClientIa::callbackCommandLevelUpIncantationEnd, "[levelUp] incantation() End");
	tmp->command = NULL;
	pushFrontCallbackCommand(tmp);
}

void	ClientIa::callbackCommandLevelUpIncantationStart(string response)
{
	cout << "incantation: " << response << endl;
	if (response == "ko\n")
	{
		pushBackCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore,
			"abandon de l'incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(incantation KO)"));
		levelUpStart(NULL);
	}
	else
		pushBackCallbackCommand(NULL, &ClientIa::callbackCommandLevelUpIncantationEnd, "[levelUp] incantationEnd()");
}

void	ClientIa::callbackContinueLevelUpCheck()
{
	if (player->getLevel() == 1)
	{
		itemsPoseStart(player->getItemsLevelUp(), &ClientIa::callbackContinueLevelUpIncantation);
	}
	else if (state == none || state == master)
	{
		//checkFood(2 * double egg burger);
		state = master;
		pushBackCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandLevelUpMasterWaitingResponses, 
			"qui veut incanter niveau " + to_string(player->getLevel() + 1) + " " + to_string(pid), "[levelUp] broadcast(qui veut incanter ?)"));
	}
}

void	ClientIa::callbackContinueLevelUpJoin()
{
	pushBackCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore, 
		"rejoignez moi pour incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(rejoignez moi)"));
	pushBackCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore, 
		"rejoignez moi pour incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(rejoignez moi)"));
	pushBackCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore, 
		"rejoignez moi pour incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(rejoignez moi)"));
	pushBackCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore, 
		"rejoignez moi pour incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(rejoignez moi)"));
	pushBackCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandLevelUpRepeatBroadcast, 
		"rejoignez moi pour incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(rejoignez moi)"));
}

void	ClientIa::callbackCommandLevelUpRepeatBroadcast(string response)
{
	(void)response;
	callbackContinueLevelUpCheck();
}

void	ClientIa::callbackCommandLevelUpMasterWaitingResponses(string response)
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
	//flagIsIncantationCaller = true;
	pushBackCallbackCommand(&ClientPlayer::incantation, &ClientIa::callbackCommandLevelUpIncantationStart, "[levelUp] incantationStart()");
}

void	ClientIa::callbackCommandLevelUpCheckBroadcastResponse(string response)
{
	(void)response;
	if (nPlayersBroadcastResponse >= player->getNPlayerLevelUp() - 1) //assez de personne pour faire une incantation
	{
		if (nPlayersReadyForIncantation == nPlayersBroadcastResponse) //tous sur zone
		{
			itemsPoseStart(player->getItemsLevelUp(), &ClientIa::callbackContinueLevelUpIncantation);
			/*cout << getpid() << "peut lancer nouvelle incantation" << endl;
			pushCallbackCommand(&ClientIa::);
			player->incantation();
			pushCallbackCommand(&ClientIa::);*/
		}
		else //assez de personne mais pas tous sur zone
		{
			callbackContinueLevelUpJoin();
			//callbackContinueLevelUpCheck();
		}
	}
	else //pas assez de personne
	{
		state = none;
		pushBackCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore,
				"abandon de l'incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(abandon pas assez de monde)"));
		if (flagFork == false)
		{
			//pushBackCallbackCommand(&ClientPlayer::fork, &ClientIa::callbackCommandIgnore, "[levelUp] fork() lay egg");
			flagFork = true;
		}
		callbackContinueLevelUpFindItem();
	}
}

void	ClientIa::callbackContinueLevelUpComming()
{
	flagGoToBroadcaster = true;
}

void	ClientIa::callbackContinueLevelUpCheckNeedFoodYes()
{
	pushFrontCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore,
		"j'arrive pour incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(j'arrive apres mangé (yes))"));
}

void	ClientIa::callbackContinueLevelUpCheckNeedFoodNo()
{
	pushBackCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore,
		"je suis sur zone level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(je suis sur zone)"));
}

void	ClientIa::receiveBroadcast(string broadcast)
{
	smatch match;

	if (regex_search(broadcast, match, regex("message ([0-8]),qui veut incanter niveau ([2-8]) ([0-9]+)\n")))
	{
		if (((state == slave || state == none) && player->getLevel() == stoi(match[2]) - 1 && stoi(match[1]) != 0) 
			|| (state == master && pid > stoi(match[3]) && player->getLevel() == stoi(match[2]) - 1))
		{
			state = slave;
			if (itemsToFind.find(FOOD) == itemsToFind.end() || itemsToFind[FOOD] == 0) // cherche pas de food
			{
				callbackCommandClear();
				stackCallbackCallerContinue.clear();
				itemsToFind.clear();
				pushFrontCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore,
					"j'arrive pour incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(j'arrive)"));
				checkStart(MIN_FOOD_2, N_TO_EAT_2, &ClientIa::callbackContinueLevelUpComming);
			}
			else // faim -> double_egg_burger(2)
			{
				flagGoToBroadcaster = false;
				int food = itemsToFind[FOOD];
				itemsToFind.clear();
				itemsToFind[FOOD] = food;
				pushFrontCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore,
					"j'arrive pour incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(j'arrive apres mangé)"));
			}
		}
		else if ((state == slave || state == none) && player->getLevel() == stoi(match[2]) - 1 && stoi(match[1]) == 0)
		{
			flagGoToBroadcaster = false;
			state = slave;
			if (itemsToFind.find(FOOD) == itemsToFind.end() || itemsToFind[FOOD] == 0) // cherche pas de food
			{
				callbackCommandClear();
				stackCallbackCallerContinue.clear();
				itemsToFind.clear();
				checkNeedFoodStart(MIN_FOOD_2, N_TO_EAT_2, &ClientIa::callbackContinueLevelUpCheckNeedFoodYes, 
					&ClientIa::callbackContinueLevelUpCheckNeedFoodNo);
			}
			else
			{
				int food = itemsToFind[FOOD];
				itemsToFind.clear();
				itemsToFind[FOOD] = food;
				pushFrontCallbackCommand(new CallbackCommand(&ClientPlayer::broadcast, &ClientIa::callbackCommandIgnore,
					"j'arrive pour incantation level " + to_string(player->getLevel() + 1), "[levelUp] broadcast(j'arrive apres mangé (sur zone))"));
			}
		}
	}
	else if (regex_search(broadcast, match, regex("message ([0-8]),abandon de l'incantation level ([2-8])\n")) && player->getLevel() == stoi(match[2]) - 1)
	{
		state = none;
		flagGoToBroadcaster = false;
		callbackCommandClear();
		stackCallbackCallerContinue.clear();
		itemsToFind.clear();
		levelUpStart(NULL);
	}
	else if (state == master && regex_search(broadcast, match, regex("message ([0-8]),j'arrive pour incantation level ([2-8])\n")))
	{
		if (player->getLevel() == stoi(match[2]) - 1)
		{
			nPlayersBroadcastResponse++;
		}
	}
	else if (state == master && regex_search(broadcast, match, regex("message ([0-8]),je suis sur zone level ([2-8])\n")))
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