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

		void								findItemStart(map<string, int> *newItemsToFind, void (ClientIa::*caller)());
		void								checkFoodStart(int minFood, int nToEat, void (ClientIa::*caller)());

	private:
		void								checkFoodEnd();
		void								endPlay();
		void								execCallbackCallerContinue();
		void								findItemSee();
		void								findItemMove();
		void								concatItemsToFind(map<string, int> *newItemsToFind);
		void								callbackCheckFoodInventory(string inventory);
		void								pushCallbackCallerContinue(void (ClientIa::*callbackCallerContinue)());
		void								pushCallbackCommand(void (ClientIa::*callbackCommand)(string reponse));
		bool								itemsToFindIsEmpty();
		void								callbackTake(string response);
		void								callbackCommandSee(string items);
		pair<string, int>					checkItemAvailable();
		void								receiveCallbackCommandIgnore(string response);
		void								printItemsToFind();
		ClientPlayer						*player;
		map<string, int>					itemsToFind;
		int									minFood;
		int									nToEat;
		pair<string, int>					itemTryToTake;
		int									nRotate;
		deque<void (ClientIa::*)(string)>	stackCallbackCommand;
		deque<void (ClientIa::*)()>			stackCallbackCallerContinue;
};

#endif
