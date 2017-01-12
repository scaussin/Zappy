#include "../includes/Client.hpp"

//------------------------------------------------------------------------------//
//																				//
//		This class takes care of the actual gameplay.							//
//																				//
//																				//
//------------------------------------------------------------------------------//

ClientPlayer::ClientPlayer()
{
	level = 1;
	lifeUnits = 10;
}

ClientPlayer::~ClientPlayer()
{}

void ClientPlayer::init(int x, int y)
{
	posX = x;
	posY = y;

	/*	std::cout << KGRN << "Player Game datas:" << KRESET << std::endl;
	std::cout << "World size: " << this->World_x << "x " << this->World_y << "y " << std::endl;
	std::cout << "Level: " << this->Level << std::endl;
	std::cout << "Life Units: " << this->LifeUnits << std::endl;
	std::cout << "Inventory size: " << this->Inventory.size() << std::endl;*/
}
