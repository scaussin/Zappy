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
