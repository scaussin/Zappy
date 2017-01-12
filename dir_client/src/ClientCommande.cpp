#include "../includes/Client.hpp"

//------------------------------------------------------------------------------//
//																				//
//		This class takes care of the actual gameplay.							//
//																				//
//																				//
//------------------------------------------------------------------------------//

ClientCommande::ClientCommande()
{
}

ClientCommande::~ClientCommande()
{}



void	ClientCommande::receiveResponse(std::string response)
{
	(this->*cmdWaitingResult.front())(response);
	//(this->*i->func)(token);
	cmdWaitingResult.pop_front();
}

