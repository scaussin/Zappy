#ifndef CLIENTPLAYER_HPP
# define CLIENTPLAYER_HPP

# include "client.hpp"

class ClientPlayer
{
	public:
		ClientPlayer(string, char **);
		~ClientPlayer();

		void						printStat();

		string						teamName;
		int							worldSizeX;
		int							worldSizeY;
		int							teamSlots;
		char						**argv;

		void						inventaire();
		void						avance();
		void						voir();
		void						gauche();
		void						droite();
		void						prend(string item);
		void						connect_nbr();
		int							move(int to);

		void						setBufferSend(ClientBuffer *buffer);
		void						setInventory(string inventory);
		map<string, int>			getInventory();
		vector<map<string, int> >	getItemsSeen();
		void						printInventory();
		void						initItemsSeen(string items);
		void						setCaseItemsSeen(int indexCase, string item, int n);
		void						printItemsSeen();
		int							getHeightVision();
		int							getLevel();

	private:
		int							level;
		int							position;
		map<string, int>			inventory;
		int							lifeUnits;
		vector<string>				vision;
		ClientBuffer				*bufferSend;
		vector<map<string, int> >	itemsSeen;

		int							poshorizontale(int pos);
		int							posverticale(int pos);
		int							memeligne(int debut, int fin);
		int							monter(int debuty, int finy, int debutx, int finx);
		int							descendre(int debuty, int finy, int debutx, int finx);


};

#endif
