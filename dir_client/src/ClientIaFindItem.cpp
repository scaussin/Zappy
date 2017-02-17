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
	player->voir();
	stackCallbackCommand.push_back(&ClientIa::callbackCommandFindItemSee);
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
		int nMove = player->move(itemAvailable.second);
		while (nMove > 0)
		{

			stackCallbackCommand.push_back(&ClientIa::callbackCommandIgnore);
			nMove--;
		}
		player->prend(itemAvailable.first);
		stackCallbackCommand.push_back(&ClientIa::callbackCommandFindItemTake);
	}
	else if (nRotate < 3)
	{
		player->droite();
		stackCallbackCommand.push_back(&ClientIa::callbackCommandIgnore);
		nRotate++;
		callbackContinueFindItemSee();
	}
	else
	{
		int nRand = rand() % 3;
		if (nRand == 0)
		{
			player->droite();
			stackCallbackCommand.push_back(&ClientIa::callbackCommandIgnore);
		}
		else if (nRand == 1)
		{
			player->gauche();
			stackCallbackCommand.push_back(&ClientIa::callbackCommandIgnore);
		}
		int nAvance = (player->getHeightVision() * 2) - 1;
		while (nAvance > 0)
		{
			player->avance();
			stackCallbackCommand.push_back(&ClientIa::callbackCommandIgnore);
			nAvance--;
		}
		findItemStart(NULL, NULL);
	}
}

void	ClientIa::callbackCommandFindItemSee(string items)
{
	player->initItemsSeen(items);
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