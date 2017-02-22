#include "clientIa.hpp"

void	ClientIa::itemsPoseStart(map<string, int> itemsToPose, void (ClientIa::*caller)())
{
	int countPush = 0;
	pushCallbackCallerContinue(caller);
	for (auto it = itemsToPose.begin(); it != itemsToPose.end(); ++it)
	{
		while (it->second > 0)
		{
			pushBackCallbackCommand(new CallbackCommand(&clientPlayer::pose, &ClientIa::callbackCommandIgnore, it->first, "itemPose pose item");
			countPush++;
			it->second--;
		}
	}
	if (countPush > 0)
	{
		stackCallbackCommand.back()->command = &ClientIa::callbackCommandItemsPoseEnd;
	}
}

void	ClientIa::callbackCommandItemsPoseEnd(string empty)
{
	(void)empty;
	execCallbackCallerContinue();
}
