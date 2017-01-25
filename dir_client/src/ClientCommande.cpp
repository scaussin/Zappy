#include "../includes/Client.hpp"

//------------------------------------------------------------------------------//
//																				//
//		player action and manage commande callback 								//
//																				//
//																				//
//------------------------------------------------------------------------------//

ClientCommande::ClientCommande()
{
}

ClientCommande::~ClientCommande()
{}

/*
** execute the first callback function.
*/
void	ClientCommande::receiveCallbackCommande(std::string response)
{
	(this->*stackCallbackCommandes.front())(response);
	stackCallbackCommandes.pop_front();
}

