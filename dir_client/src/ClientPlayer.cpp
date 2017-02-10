#include "client.hpp"

//------------------------------------------------------------------------------//
//																				//
//		This class takes care of the actual gameplay.							//
//																				//
//																				//
//------------------------------------------------------------------------------//

ClientPlayer::ClientPlayer(string _teamName) : teamName(_teamName)
{
	level = 1;
	lifeUnits = 10;
	teamSlots = -1;
}

ClientPlayer::~ClientPlayer()
{}

void	ClientPlayer::printStat()
{
	cout << KGRN << "Player Game datas:" << KRESET << endl;
	cout << "  World size: " << worldSizeX << "x " << worldSizeY << "y " << endl;
	cout << "  Team: " << teamName << " - Level: " << level << endl;
	cout << "  Life Units: " << lifeUnits << endl;
	cout << "  Inventory size: " << inventory.size() << endl;
}

void	ClientPlayer::inventaire()
{
	bufferSend->pushMsg("inventaire\n");
}

void	ClientPlayer::voir()
{
	bufferSend->pushMsg("voir\n");
}

void	ClientPlayer::avance()
{
	bufferSend->pushMsg("avance\n");
}

void	ClientPlayer::setInventory(string inventory)
{
	char	*item;
	char	*n;
	char	*dup;

	inventory = inventory.substr(1, inventory.size() - 2);
	dup = strdup(inventory.c_str());
	item = strtok(dup, ", ");
	while (item)
	{
		n = strtok(NULL, ", ");
		if (n)
		{
			this->inventory[item] = atoi(n);
			item = strtok(NULL, ", ");
		}
		else
			break ;
	}
	free(dup);
}

map<string, int>	ClientPlayer::getInventory()
{
	return (inventory);
}

void	ClientPlayer::printInventory()
{
	for(map<string, int>::const_iterator it = inventory.cbegin(); it != inventory.cend(); ++it)
	{
		cout << it->first << " " << it->second << endl;
	}
}

void	ClientPlayer::setBufferSend(ClientBuffer *buffer)
{
	bufferSend = buffer;
}


void	ClientPlayer::setItemsSeen(string items)
{
	/*(void)items;*/
	map<string, int>	mapCase;
	char				*caseItems;
	char				*dup;
	char				*item;
	char				*lstItems;

	itemsSeen.clear();
	items = items.substr(1, items.size() - 2);
	dup = strdup(items.c_str());
	caseItems = strtok(dup, ",");
	while (caseItems)
	{
		lstItems = strdup(caseItems);
		mapCase.clear();
		item = strtok(lstItems, " ");
		while (item)
		{
			if (mapCase.find(item) == mapCase.end())
				mapCase[item] = 1;
			else
				mapCase[item]++;
			item = strtok(NULL, " ");
		}
		itemsSeen.push_back(mapCase);
		free(lstItems);
		lstItems = NULL;
		caseItems = strtok(NULL, ",");
	}
	free(dup);
}

void	ClientPlayer::printItemsSeen()
{
	unsigned int		i = 0;

	while (i < itemsSeen.size())
	{
		for(map<string, int>::iterator it = itemsSeen[i].begin(); it != itemsSeen[i].end(); ++it)
		{
			cout << it->first << ":" << it->second << "  ";
		}
		cout << endl;
		i++;
	}
}