#ifndef CLIENTPLAYER_HPP
# define CLIENTPLAYER_HPP

# include "Client.hpp"

class ClientPlayer
{
	public:
		ClientPlayer(string teamName);
		~ClientPlayer();

		void						printStat();

		string					teamName;
		int							worldSizeX;
		int							worldSizeY;
		int							teamSlots;

		/*void						avance(void (ClientIa::*)(string));
		void						droite(void (ClientIa::*)(string));
		void						gauche(void (ClientIa::*)(string));
		void						voir(void (ClientIa::*)(string));*/
		//void						inventaire(void (ClientIa::*)(string));
		void						inventaire();
		void						setBufferSend(ClientBuffer *buffer);

		

	private:
		int							level;
		int							lifeUnits;
		list<string>		inventory;
		list<string>		vision;
		ClientBuffer				*bufferSend;

};

#endif
