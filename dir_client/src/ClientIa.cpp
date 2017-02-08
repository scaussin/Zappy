#include "../includes/Client.hpp"

ClientIa::ClientIa(ClientPlayer *_player) : player(_player)
{
}

ClientIa::~ClientIa()
{
}

void	ClientIa::callbackInventaire(string inventaire)
{
	cout << "inventaire: " << inventaire << endl;
}

void	ClientIa::findItem(/*items[]*/)
{
	player->inventaire();
	stackCallbackCommandes.push_back(&ClientIa::callbackInventaire);
}


void	ClientIa::receiveCallbackBroadcast(string broadcast)
{
}

void	ClientIa::receiveCallbackCommande(string response)
{
	(this->*stackCallbackCommandes.front())(response);
	stackCallbackCommandes.pop_front();
}