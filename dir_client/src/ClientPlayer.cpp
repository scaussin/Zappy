#include "../includes/Client.hpp"

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

void	ClientPlayer::setBufferSend(ClientBuffer *buffer)
{
	bufferSend = buffer;
}

/*
void	ClientPlayer::avance()
{
	bufferSend->pushMsg("avance\n");
	stackCallbackCommandes.push_back(&ClientCommande::avanceCallback);
}

void	ClientPlayer::droite()
{
	bufferSend->pushMsg("gauche\n");
	stackCallbackCommandes.push_back(&ClientCommande::droiteCallback);
}


void	ClientPlayer::gauche()
{
	bufferSend->pushMsg("gauche\n");
	stackCallbackCommandes.push_back(&ClientCommande::gaucheCallback);
}

void	ClientPlayer::voir()
{
	bufferSend->pushMsg("voir\n");
	stackCallbackCommandes.push_back(&ClientCommande::voirCallback);
}*/
/*
void	ClientPlayer::inventaire(void (ClientIa::*callback)(string))
{
	bufferSend->pushMsg("inventaire\n");
	stackCallbackCommandes.push_back(&ClientCommande::voirCallback);
}
*/
/*
** Methodes callback
*/
/*
void	ClientPlayer::avanceCallback(string response)
{
	cout << "exec avance ok" << endl;
	avance();
	gauche();
}

void	ClientPlayer::droiteCallback(string response)
{
	cout << "exec droite ok" << endl;
}

void	ClientPlayer::gaucheCallback(string response)
{
	cout << "exec gauche ok" << endl;
}

void	ClientPlayer::voirCallback(string response)
{
	cout << "exec voir ok" << endl;
}*/