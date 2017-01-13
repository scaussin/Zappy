#include "../includes/Client.hpp"

ClientController::ClientController(int argc, char **argv)
{
	//std::cout << KCYN "Client Controller starting..." KRESET << std::endl;
	try
	{
		inputHandler = new ClientInputHandler(argc, argv);
		player = new ClientPlayer(inputHandler->teamName);
		communication = new ClientCommunication(inputHandler->hostName, inputHandler->port, player);
	}
	catch (CustomException &e)
	{
		std::cout << KRED << e.what() << KRESET << std::endl;
		std::cout << std::endl << KMAG << "Usage: ./client -n <team> -p <port> [-h <hostname>]" << KRESET << std::endl;
		return ;
	}
	try
	{
		communication->connectToServer();
		std::cout << KGRN << "connected" << KRESET << std::endl;
	}
	catch (CustomException &e)
	{
		std::cout << KRED << e.what() << KRESET << std::endl;
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
			std::cout << KRED << e.what() << KRESET << std::endl;
			return ;
		}
	}
}
