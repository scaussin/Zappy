#include "client.hpp"

ClientController::ClientController(int argc, char **argv)
{
	//cout << KCYN "Client Controller starting..." KRESET << endl;
	try
	{
		inputHandler = new ClientInputHandler(argc, argv);
		player = new ClientPlayer(inputHandler->teamName, argv);
		ia = new ClientIa(player);
		communication = new ClientCommunication(inputHandler->hostName, inputHandler->port, player, ia);
	}
	catch (CustomException &e)
	{
		cout << KRED << e.what() << KRESET << endl;
		cout << endl << KMAG << "Usage: ./client -n <team> -p <port> [-h <hostname>]" << KRESET << endl;
		return ;
	}
	try
	{
		communication->connectToServer();
		cout << KGRN << "connected" << KRESET << endl;
	}
	catch (CustomException &e)
	{
		cout << KRED << e.what() << KRESET << endl;
		return ;
	}
}

ClientController::~ClientController()
{}

void	ClientController::mainLoop()
{
	while (42)
	{
		try
		{
			communication->initFd();
			communication->doSelect();
			if (communication->checkFd() == -1)
				break ;
			communication->manageRecv();
		}
		catch (CustomException &e)
		{
			cout << KRED << e.what() << KRESET << endl;
			return ;
		}
	}
}
