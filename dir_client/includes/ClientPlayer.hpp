#ifndef CLIENTPLAYER_HPP
# define CLIENTPLAYER_HPP

# include "Client.hpp"

class ClientPlayer
{
	public:
		ClientConnection						*Connection;

		int										World_x;
		int										World_y;

		int										Level;
		int										LifeUnits;

		std::list<std::string>					Inventory;
		std::list<std::string>					Vision;

		ClientPlayer();
		~ClientPlayer();

		void		InitGameDatas(ClientConnection &SentConnection);

};

#endif
