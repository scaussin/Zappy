#include "ClientIa.hpp"

void	ClientIa::newClientStart(void (ClientIa::*caller)())
{
	pushCallbackCallerContinue(caller);
	pushBackCallbackCommand(&ClientPlayer::connect_nbr, &ClientIa::callbackCommandConnectNbr, "[newClient] connect_nbr()");
}

void	ClientIa::callbackCommandConnectNbr(string connectNbr)
{
	(string)connectNbr;
	if (connectNbr == "ko\n")
	{
		cout << KRED << "error callbackCommandConnectNbr()" << getpid() << KRESET << endl;
		//stackCallbackCommand.push_front(&ClientIa::callbackCommandConnectNbr);
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