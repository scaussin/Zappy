#include "../includes/Client.hpp"

ClientController::ClientController(int argc, char **argv)
{
	//std::cout << KCYN "Client Controller starting..." KRESET << std::endl;
	try
	{
		player = new ClientPlayer();
		inputHandler = new ClientInputHandler(argc, argv);
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
		communication->connect();
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
			communication->checkFd();
		}
		catch (CustomException &e)
		{
			std::cout << KRED << e.what() << KRESET << std::endl;
			return ;
		}
	}
}
