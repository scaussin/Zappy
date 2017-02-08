#ifndef CLIENTIA_HPP
# define CLIENTIA_HPP

# include "Client.hpp"

class ClientIa
{
	public:
		ClientIa(ClientPlayer *player);
		~ClientIa();
		void						receiveCallbackCommande(string response);
		void						receiveCallbackBroadcast(string broadcast);
		void						findItem();
		void						callbackInventaire(string inventaire);

	private:
		ClientPlayer				*player;
		deque<void (ClientIa::*)(string)>	stackCallbackCommandes;
};

#endif
