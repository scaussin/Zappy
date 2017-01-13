#include "../includes/Client.hpp"

//------------------------------------------------------------------------------//
//																				//
//		This class takes care of the actual gameplay.							//
//																				//
//																				//
//------------------------------------------------------------------------------//

ClientPlayer::ClientPlayer(std::string _teamName) : teamName(_teamName)
{
	level = 1;
	lifeUnits = 10;
	teamSlots = -1;
}

ClientPlayer::~ClientPlayer()
{}

void	ClientPlayer::printStat()
{
	std::cout << KGRN << "Player Game datas:" << KRESET << std::endl;
	std::cout << "  World size: " << worldSizeX << "x " << worldSizeY << "y " << std::endl;
	std::cout << "  Team: " << teamName << " - Level: " << level << std::endl;
	std::cout << "  Life Units: " << lifeUnits << std::endl;
	std::cout << "  Inventory size: " << inventory.size() << std::endl;
}

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
}

/*
** Methodes callback
*/

void	ClientPlayer::avanceCallback(std::string response)
{
	std::cout << "exec avance ok" << std::endl;
}

void	ClientPlayer::droiteCallback(std::string response)
{
	std::cout << "exec droite ok" << std::endl;
}

void	ClientPlayer::gaucheCallback(std::string response)
{
	std::cout << "exec gauche ok" << std::endl;
}

void	ClientPlayer::voirCallback(std::string response)
{
	std::cout << "exec voir ok" << std::endl;
}