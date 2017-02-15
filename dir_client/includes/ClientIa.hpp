#ifndef CLIENTIA_HPP
# define CLIENTIA_HPP

# include "client.hpp"

class ClientIa
{
	public:
		ClientIa(ClientPlayer *player);
		~ClientIa();
		void								startPlay();
		void								receiveCallbackCommand(string response);
		void								receiveCallbackBroadcast(string broadcast);

	private:
		void								findItemStart(map<string, int> *newItemsToFind, void (ClientIa::*caller)());
		void								checkStart(int minFood, int nToEat, void (ClientIa::*caller)());
		void								newClientStart(void (ClientIa::*caller)());
		void								levelUpStart(void (ClientIa::*caller)());
		void								checkEnd();
		void								endPlay();
		void								newClientEnd();
		void								execCallbackCallerContinue();
		void								callbackContinueFindItemSee();
		void								findItemMove();
		void								concatItemsToFind(map<string, int> *newItemsToFind);
		void								callbackCommandCheckInventory(string inventory);
		void								pushCallbackCallerContinue(void (ClientIa::*callbackCallerContinue)());
		void								pushCallbackCommand(void (ClientIa::*callbackCommand)(string reponse));
		bool								itemsToFindIsEmpty();
		void								callbackCommandFindItemTake(string response);
		void								callbackCommandFindItemSee(string items);
		pair<string, int>					checkItemAvailable();
		void								receiveCallbackCommandIgnore(string response);
		void								printItemsToFind();
		void								callbackCommandConnectNbr(string connectNbr);
		void								callbackContinueCheckInventory();
		void								callbackCommandLevelUpInventory(string inventory);
		void								callbackContinueLevelUpFindItem();
		void								callbackContinueLevelUpCheck();
		ClientPlayer						*player;
		map<string, int>					itemsToFind;
		int									minFood;
		int									nToEat;
		pair<string, int>					itemTryToTake;
		int									nRotate;
		int									nBroadcast;
		deque<void (ClientIa::*)(string)>	stackCallbackCommand;
		deque<void (ClientIa::*)()>			stackCallbackCallerContinue;
};

#endif
