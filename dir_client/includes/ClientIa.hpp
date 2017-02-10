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

		void								findItemStart(map<string, int> itemsToFind, void (ClientIa::*caller)());
		void								checkFoodStart(int minFood, int nToEat, void (ClientIa::*caller)());

	private:
		void								checkFoodEnd();
		void								endPlay();
		void								execCallbackCallerContinue();
		void								findItemSee();
		void								callbackInventory(string inventory);
		void								callbackCheckFoodInventory(string inventory);
		void								pushCallbackCallerContinue(void (ClientIa::*callbackCallerContinue)());
		void								pushCallbackCommand(void (ClientIa::*callbackCommand)(string reponse));
		bool								itemsToFindIsEmpty();
		void								callbackFindItemSee(string items);
		int									checkItemAvailable();
		ClientPlayer						*player;
		map<string, int>					itemsToFind;
		int									minFood;
		int									nToEat;
		int									nRotate;
		deque<void (ClientIa::*)(string)>	stackCallbackCommand;
		deque<void (ClientIa::*)()>			stackCallbackCallerContinue;
};

#endif
