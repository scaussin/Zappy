#include "ClientIa.hpp"

void	ClientIa::newClientStart(void (ClientIa::*caller)())
{
	pushCallbackCallerContinue(caller);
	pushBackCallbackCommand(&ClientPlayer::connect_nbr, &ClientIa::callbackCommandConnectNbr, "[newClient] connect_nbr()");
}

void	ClientIa::callbackCommandConnectNbr(string connectNbr)
{
	int nb = 0;

	try
	{
		nb = stoi(connectNbr);
	}
	catch (invalid_argument e)
	{
		cout << KRED << "[ERROR] connect_nbr: invalid arg" << KRESET << endl;
	}

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