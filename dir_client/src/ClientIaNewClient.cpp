#include "ClientIa.hpp"

void	ClientIa::newClientStart(void (ClientIa::*caller)())
{
	pushCallbackCallerContinue(caller);
	player->connect_nbr();
	stackCallbackCommand.push_back(&ClientIa::callbackCommandConnectNbr);
}

void	ClientIa::callbackCommandConnectNbr(string connectNbr)
{
	if (connectNbr == "ko\n")
	{
		cout << "qwertyuiop " << getpid() <<endl;
		stackCallbackCommand.push_front(&ClientIa::callbackCommandConnectNbr);
		return ;
	}
	cout << "connect_nbr: " << connectNbr << endl;
	int nb = stoi(connectNbr);

	if (nb > 0)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			// child
			execve(player->argv[0], player->argv, NULL);
		}
	}
	newClientEnd();
}

void	ClientIa::newClientEnd()
{
	execCallbackCallerContinue();
}