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

		void						inventaire(string noArg);
		void						avance(string noArg);
		void						voir(string noArg);
		void						gauche(string noArg);
		void						droite(string noArg);
		void						prend(string item);
		void						pose(string item);
		void						connect_nbr(string noArg);
		void						broadcast(string broadcast);
		void						incantation(string noArg);
		void						fork(string noArg);

		vector<void (ClientPlayer::*)(string)>	move(int to);

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
		//void						init(void (ClientIa::**nextCallbackCommand)(string));

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
		/*void 						(ClientIa::**nextCallbackCommand)(string);
		deque<void (ClientIa::*)(string)>	*iaStackCallbackCommand;*/

		int							poshorizontale(int pos);
		int							posverticale(int pos);
		void						memeligne(int debut, int fin, vector<void (ClientPlayer::*)(string)> *cmdMove);
		void						monter(int debuty, int finy, int debutx, int finx, vector<void (ClientPlayer::*)(string)> *cmdMove);
		void						descendre(int debuty, int finy, int debutx, int finx, vector<void (ClientPlayer::*)(string)> *cmdMove);
		
};

#endif
