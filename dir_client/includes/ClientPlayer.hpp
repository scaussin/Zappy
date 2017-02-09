#ifndef CLIENTPLAYER_HPP
# define CLIENTPLAYER_HPP

# include "Client.hpp"

class ClientPlayer
{
	public:
		ClientPlayer(string teamName);
		~ClientPlayer();

		void						printStat();

		string						teamName;
		int							worldSizeX;
		int							worldSizeY;
		int							teamSlots;

		/*void						avance(void (ClientIa::*)(string));
		void						droite(void (ClientIa::*)(string));
		void						gauche(void (ClientIa::*)(string));
		void						voir(void (ClientIa::*)(string));*/
		//void						inventaire(void (ClientIa::*)(string));
		void						inventaire();
		void						avance();
		void						voir();

		void						setBufferSend(ClientBuffer *buffer);
		void						setInventory(string inventory);
		map<string, int>			getInventory();
		void						printInventory();

	private:
		map<string, int>			inventory;
		int							level;
		int							lifeUnits;
		vector<string>				vision;
		ClientBuffer				*bufferSend;


};

#endif
