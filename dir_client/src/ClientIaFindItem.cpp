#include "ClientIa.hpp"

void	ClientIa::findItemStart(map<string, int> *newItemsToFind, void (ClientIa::*caller)())
{
	pushCallbackCallerContinue(caller);
	nRotate = 0;
	concatItemsToFind(newItemsToFind);
	if (this->itemsToFind.find(FOOD) == this->itemsToFind.end() || this->itemsToFind[FOOD] <= 0)
		checkStart(MIN_FOOD_1, N_TO_EAT_1, &ClientIa::callbackContinueFindItemSee);
	else
		callbackContinueFindItemSee();
}

void	ClientIa::concatItemsToFind(map<string, int> *newItemsToFind)
{
	if (newItemsToFind)
	{
		for (map<string, int>::iterator it = newItemsToFind->begin() ; it != newItemsToFind->end() ; ++it)
		{
			if (itemsToFind.find(it->first) != itemsToFind.end())
				itemsToFind[it->first] += it->second;
			else
				itemsToFind[it->first] = it->second;
		}
	}
}

void	ClientIa::callbackContinueFindItemSee()
{
	pushBackCallbackCommand(&ClientPlayer::voir, &ClientIa::callbackCommandFindItemSee, "[findItem] voir()");
}

void	ClientIa::callbackCommandFindItemTake(string response)
{
	if (response == SUCCESS)
	{
		itemsToFind[itemTryToTake.first]--;
		player->setCaseItemsSeen(itemTryToTake.second, itemTryToTake.first,	player->getItemsSeen()[itemTryToTake.second][itemTryToTake.first] - 1);
		cout << itemTryToTake.first << ": " << player->getItemsSeen()[itemTryToTake.second][itemTryToTake.first] << endl;
		//printItemsToFind();
	}
	else
		player->setCaseItemsSeen(itemTryToTake.second, itemTryToTake.first, 0);
	findItemMove();
}

void	ClientIa::findItemMove()
{
	if (itemsToFindIsEmpty())
	{
		execCallbackCallerContinue();
		return ;
	}
	pair<string, int> itemAvailable = checkItemAvailable();
	if (itemAvailable.second != -1) //move
	{
		itemTryToTake = itemAvailable;
		vector<void (ClientPlayer::*)(string)> cmdMove = player->move(itemAvailable.second);
		for (auto it = cmdMove.begin() ; it != cmdMove.end(); ++it)
		{
			pushBackCallbackCommand(*it, &ClientIa::callbackCommandIgnore, "[findItem] move to item");
		}
		pushBackCallbackCommand(new CallbackCommand(&ClientPlayer::prend, &ClientIa::callbackCommandFindItemTake, itemAvailable.first, "[findItem] take item"));
	}
	else if (nRotate < 3)
	{
		pushBackCallbackCommand(&ClientPlayer::droite, &ClientIa::callbackCommandIgnore, "[findItem] droite (rotate)");
		nRotate++;
		callbackContinueFindItemSee();
	}
	else
	{
		int nRand = rand() % 3;
		if (nRand == 0)
			pushBackCallbackCommand(&ClientPlayer::droite, &ClientIa::callbackCommandIgnore, "[findItem] droite (rand)");
		else if (nRand == 1)
			pushBackCallbackCommand(&ClientPlayer::gauche, &ClientIa::callbackCommandIgnore, "[findItem] gauche (rand)");
		int nAvance = (player->getHeightVision() * 2) - 1;
		while (nAvance > 0)
		{
			pushBackCallbackCommand(&ClientPlayer::avance, &ClientIa::callbackCommandIgnore, "[findItem] avance (to another spot)");
			nAvance--;
		}
		findItemStart(NULL, NULL);
	}
}

void	ClientIa::callbackCommandFindItemSee(string items)
{
	player->initItemsSeen(items);
	if (items == "ok\n")
	{
		cout << "error voir" << endl;
		exit(0);
	}
	//player->printItemsSeen();
	findItemMove();
}

bool	ClientIa::itemsToFindIsEmpty()
{
	for (map<string, int>::iterator it = itemsToFind.begin(); it != itemsToFind.end(); ++it)
	{
		if (it->second > 0)
			return false;
	}
	return true;
}

void	ClientIa::printItemsToFind()
{
	cout<<KRED<<"items to find: " << endl;
	for(map<string, int>::const_iterator it = itemsToFind.cbegin(); it != itemsToFind.cend(); ++it)
	{
		cout <<it->first << " " << it->second << endl;
	}
	cout <<KRESET;
}

pair<string, int>	ClientIa::checkItemAvailable()
{
	size_t						i = 0;
	vector<map<string, int> >	itemsSeen;

	itemsSeen = player->getItemsSeen();
	while (i < player->getItemsSeen().size())
	{
		for (map<string, int>::iterator item = itemsToFind.begin() ; item != itemsToFind.end() ; ++item)
		{
			if (itemsSeen[i].find(item->first) != itemsSeen[i].end() && itemsSeen[i][item->first] > 0 && itemsToFind[item->first] > 0)
				return (make_pair(item->first, i));
		}
		i++;
	}
	return make_pair("", -1);
}