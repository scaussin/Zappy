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

void	ClientPlayer::droite()
{
	bufferSend->pushMsg("droite\n");
}

void	ClientPlayer::gauche()
{
	bufferSend->pushMsg("gauche\n");
}

void	ClientPlayer::prend(string item)
{
	bufferSend->pushMsg("prend " + item + "\n");
}

int		ClientPlayer::move(int to)
{
	int nMove = 0;
	int posxdebut = poshorizontale(position);
	int posydebut = posverticale(position);
	int posxfin = poshorizontale(to);
	int posyfin = posverticale(to);
	int monte = posyfin - posydebut;

	if (monte == 0)
		nMove += memeligne(posxdebut, posxfin);
	if (monte > 0)
		nMove += monter(posydebut, posyfin, posxdebut, posxfin);
	if (monte < 0)
		nMove += descendre(posydebut, posyfin, posxdebut, posxfin);
	position = to;
	return (nMove);
}


int		ClientPlayer::poshorizontale(int pos)	//return pos par rapport au milieu
{
	int milieu = 0;
	int debutline = 0;
	int nbcaseinline = 0;
	int carre = 0;
	while (carre * carre < pos)
		carre++;
	if (carre * carre > pos)
		carre--;	// calcul dans quelle ligne on est
	nbcaseinline = carre * 2 + 1;
	debutline = carre * carre;
	milieu = debutline + carre;
	while (debutline != pos)
	{
		debutline++;
	}
	return (debutline - milieu);
}

int		ClientPlayer::posverticale(int pos)	//return ligne
{
	int carre = 0;
	while (carre * carre < pos)
		carre++;
	if (carre * carre > pos)
		carre--;
	return (carre);
}

int		ClientPlayer::memeligne(int debut, int fin)
{
	int nMove = 0;

	if (debut == fin)
		return (nMove);
	if (debut < fin)
	{
		droite();
		while (debut != fin)
		{
			nMove++;
			debut++;
			avance();
		}
		gauche();
	}
	else if (debut > fin)
	{
		gauche();
		while (debut != fin)
		{
			debut--;
			nMove++;
			avance();
		}
		droite();
	}
	return (nMove + 2);
}

int		ClientPlayer::monter(int debuty, int finy, int debutx, int finx)
{
	int nMove = 0;

	while (debuty < finy)
	{
		debuty++;
		nMove++;
		avance();
	}
	return (memeligne(debutx, finx) + nMove);
}

int		ClientPlayer::descendre(int debuty, int finy, int debutx, int finx)
{
	int nMove = 0;

	nMove = memeligne(debutx, finx);
	droite();
	droite();
	while (debuty > finy)
	{
		debuty--;
		nMove++;
		avance();
	}
	droite();
	droite();
	return(nMove + 4);
}

void	ClientPlayer::setInventory(string inventory)
{
	char	*item;
	char	*n;
	char	*dup;

	inventory = inventory.substr(1, inventory.size() - 3);
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

void	ClientPlayer::setCaseItemsSeen(int indexCase, string item, int n)
{
	itemsSeen[indexCase][item] = n;
}

void	ClientPlayer::initItemsSeen(string items)
{
	map<string, int>	mapCase;
	vector<string>		casesItems;
	char				*lstItems;
	char				*item;

	position = 0;
	itemsSeen.clear();
	items = items.substr(1, items.size() - 3);
	casesItems = strSplit(items, ',');
	for (vector<string>::iterator it = casesItems.begin() ; it != casesItems.end() ; ++it)
	{
		lstItems = strdup(it->c_str());
		item = strtok(lstItems, " ");
		mapCase.clear();
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
	}
}

void	ClientPlayer::printItemsSeen()
{
	unsigned int		i = 0;

	cout << KRED << "voir : " << KRESET << endl;
	while (i < itemsSeen.size())
	{
		for(map<string, int>::iterator it = itemsSeen[i].begin(); it != itemsSeen[i].end(); ++it)
		{
			cout << KCYN << it->first << ": " <<KRESET<< it->second << " | ";
		}
		cout << endl;
		i++;
	}
}

int		ClientPlayer::getHeightVision()
{
	return (level + 1);
}

vector<map<string, int> >	ClientPlayer::getItemsSeen()
{
	return (itemsSeen);
}
