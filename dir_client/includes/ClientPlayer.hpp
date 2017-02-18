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
		void						pose(string item);
		void						connect_nbr();
		void						broadcast(string broadcast);
		int							move(int to);
		void						incantation();
		void						fork();

		void						setBufferSend(ClientBuffer *buffer);
		void						setInventory(string inventory);
		map<string, int>			getInventory();
		vector<map<string, int> >	getItemsSeen();
		map<string, int>			getItemsLevelUp();
		void						printInventory();
		void						initItemsSeen(string items);
		void						setCaseItemsSeen(int indexCase, string item, int n);
		void						printItemsSeen();
		void						printVectMap(vector<map<string, int> > items);
		void						printMap(map<string, int> items);
		int							getHeightVision();
		int							getNPlayerLevelUp();
		int							getLevel();
		void						setLevel(int _level);

	private:
		int							level;
		int							position;
		map<string, int>			inventory;
		//int							lifeUnits;
		//vector<string>				vision;
		ClientBuffer				*bufferSend;
		vector<map<string, int> >	itemsSeen;
		vector<map<string, int> >	levelUpItems;
		vector<int>					nPlayersLevelUp;

		int							poshorizontale(int pos);
		int							posverticale(int pos);
		int							memeligne(int debut, int fin);
		int							monter(int debuty, int finy, int debutx, int finx);
		int							descendre(int debuty, int finy, int debutx, int finx);
};

#endif
