#ifndef CLIENTIA_HPP
# define CLIENTIA_HPP

# include "client.hpp"

enum incantationState 
{
	none,
	master,
	slave
};

class ClientIa
{
	public:
		ClientIa(ClientPlayer *player);
		~ClientIa();
		void								startPlay();
		void								receiveCallbackCommand(string response);
		void								receiveBroadcast(string broadcast);
		void								pushFrontElevationEnd();
		incantationState					state;
		//bool								flagIsIncantationCaller;
		void								printStateIa(string);
		
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
		bool								itemsToFindIsEmpty();
		void								callbackCommandFindItemTake(string response);
		void								callbackCommandFindItemSee(string items);
		pair<string, int>					checkItemAvailable();
		void								callbackCommandIgnore(string response);
		void								printItemsToFind();
		void								subItemsToFind(map<string, int> *newItemsToFind);
		void								callbackCommandLevelUpMasterWaitingResponses(string response);
		void								callbackCommandConnectNbr(string connectNbr);
		void								callbackContinueCheckInventory();
		void								callbackCommandLevelUpInventory(string inventory);
		void								callbackContinueLevelUpFindItem();
		void								callbackContinueLevelUpCheck();
		void								callbackContinueLevelUpNewClient();
		void								callbackCommandLevelUpCheckBroadcastResponse(string response);
		void								callbackCommandLevelUpIncantationStart(string response);
		void								callbackCommandLevelUpIncantationEnd(string response);
		void								callbackCommandLevelUpRepeatBroadcast(string response);
		void								goToBroadcaster(int target);
		void								itemsPoseStart(map<string, int> itemsToPose, void (ClientIa::*caller)());
		void								callbackCommandItemsPoseEnd(string empty);
		void								callbackContinueLevelUpIncantation();
		//void								ignoreCallbackCommand();
		void								checkBeforeBroadcastStart(int minFood, int nToEat, void (ClientIa::*caller)());
		void								callbackCommandCheckBeforeBroadcastInventory(string inventory);
		void								callbackContinueCheckBeforeBroadcastEnd();
		void								pushBackCallbackCommand(CallbackCommand *callbackCommand);
		void								pushFrontCallbackCommand(CallbackCommand *callbackCommand);
		void								pushBackCallbackCommand(void (ClientPlayer::*command)(string), void (ClientIa::*callback)(string), string debug);
		void								sendNextCommand();
		void								callbackContinueLevelUpComming();
		void								callbackContinueLevelUpJoin();
		void								callbackCommandcheckNeedFoodInventory(string inventory);
		void								loopCheckFood();
		void								callbackCommandClear();
		void								callbackContinueLevelUpCheckNeedFoodYes();
		void								callbackContinueLevelUpCheckNeedFoodNo();
		void								checkNeedFoodStart(int minFood, int nToEat, void (ClientIa::*yes)(), void (ClientIa::*no)(), void (ClientIa::*end)());
		void								callbackCommandLevelUpBroadcastResponseCheckNeedFoodYes();
		void								callbackCommandLevelUpBroadcastResponseCheckNeedFoodNo();
		bool								isInLoopCheckFood();

		//bool								flagIsSlave;
		bool								flagGoToBroadcaster;
		bool								flagFork;
		//int									nCommandIgnore;
		
		pid_t								pid;
		ClientPlayer						*player;
		map<string, int>					itemsToFind;
		int									minFood;
		int									nToEat;

		int									nPlayersBroadcastResponse;
		int									nPlayersReadyForIncantation;
		pair<string, int>					itemTryToTake;
		int									nRotate;
		//deque<void (ClientIa::*)(string)>	stackCallbackCommand;

		CallbackCommand						*curCallbackCommand;
		deque<CallbackCommand*>				stackCallbackCommand;

		deque<void (ClientIa::*)()>			stackCallbackCallerContinue;

		
};

#endif
