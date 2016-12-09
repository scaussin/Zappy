#include "../includes/Client.hpp"

// Default Constructor
ClientController::ClientController()
{
	std::cout << KCYN "Client Controller starting..." KRESET << std::endl;
	IsConnected = false;
}

// Default destructor
ClientController::~ClientController()
{


}

// First method called from the program.
void	ClientController::OnInitialize(int argc, char **argv)
{
	//------------------------------------------------------//
	// I. get input from prog exec 							//
	//	-> goes into Connection.Settings struct;			//
	//------------------------------------------------------//
	try
	{
		// ClientInputHandler.cpp
		InputHandler.GetInput(this->Connection, argc, argv);
	}
	catch (CustomException &e)
	{
		std::cout << KRED << e.what() << KRESET << std::endl;
		std::cout << std::endl << KMAG << "Usage: ./client -n <team> -p <port> [-h <hostname>]" << KRESET << std::endl;
		return ;
	}
	
	//------------------------------------------------------//
	// II. try to connect to serveur						//
	//------------------------------------------------------//
	try
	{
		// ClientConnection.cpp
		Connection.DisplayInfos();
		Connection.SetMockMode(false);
		Connection.Connect();
	}
	catch (CustomException &e)
	{
		std::cout << KRED << e.what() << KRESET << std::endl;
		return ;
	}
}
