#include "../includes/Client.hpp"

//------------------------------------------------------------------------------//
//																				//
//		This class takes care of the actual gameplay.							//
//																				//
//																				//
//------------------------------------------------------------------------------//

ClientPlayer::ClientPlayer() : Connection(NULL)
{

}


ClientPlayer::~ClientPlayer()
{

}

// First, lets set all games elements as they should be at game beginning.
void	ClientPlayer::InitGameDatas(ClientConnection &SentConnection)
{
	// with this, ClientPlayer methods have access 
	// to the connection class to send msgs.
	this->Connection = &SentConnection;

	// fill the player position with the received string during connection.
	this->World_x = this->Connection->Startx;
	this->World_y = this->Connection->Starty;

	this->Level = 1;
	this->LifeUnits = 10;

	// Starting print
	std::cout << KGRN << "Player Game datas:" << KRESET << std::endl;
	std::cout << "World size: " << this->World_x << "x " << this->World_y << "y " << std::endl;
	std::cout << "Level: " << this->Level << std::endl;
	std::cout << "Life Units: " << this->LifeUnits << std::endl;
	std::cout << "Inventory size: " << this->Inventory.size() << std::endl;
}
