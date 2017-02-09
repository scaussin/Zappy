#ifndef CLIENTIA_HPP
# define CLIENTIA_HPP

# include "Client.hpp"

class ClientIa
{
	public:
		ClientIa(ClientPlayer *player);
		~ClientIa();
		void						startPlay();
		void						receiveCallbackCommand(string response);
		void						receiveCallbackBroadcast(string broadcast);
		
		void						findItemStart(map<string, int> itemsToFind);
		void						checkFoodStart(int minFood, int nToEat);

	private:
		void						checkFoodEnd();
		void						endPlay();
		void						execCallbackEnd();
		void						findItemSee();
		void						callbackInventory(string inventory);
		void						callbackCheckFoodInventory(string inventory);
		void						pushCallbackEnd(void (ClientIa::*callbackEnd)());
		void						pushCallbackCommand(void (ClientIa::*callbackCommand)(string reponse));
		bool						itemsToFindIsEmpty();
		void						callbackFindItemSee(string items);
		ClientPlayer				*player;
		map<string, int>			itemsToFind;
		int							minFood;
		int							nToEat;
		int							nRotate;
		deque<void (ClientIa::*)(string)>	stackCallbackCommand;
		deque<void (ClientIa::*)()>	stackCallbackEnd;
};

#endif
