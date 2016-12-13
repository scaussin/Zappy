#include "../includes/Client.hpp"

//------------------------------------------------------------------------------//
//																				//
//		This class takes care of connection and communication with the			//
//		server.																	//
//																				//
//------------------------------------------------------------------------------//

ClientConnection::ClientConnection()
{
	Settings.TeamName = "unset";
	Settings.Port = -1;
	Settings.HostName = "unset";
	this->IsInMockMode = false;
	this->IsConnected = false;
}

ClientConnection::~ClientConnection()
{


}

void	ClientConnection::Connect()
{
	std::cout << KCYN "Connection attempt..." KRESET << std::endl;
	
	if (!IsConnected)
	{
		if (IsInMockMode)
		{
			std::cout << KMAG "Engaging MOCK CONNECTION..." KRESET << std::endl;
			this->IsConnected = true;
		}
		else
		{
			std::cout << KCYN "Engaging REAL CONNECTION..." KRESET << std::endl;
			// TODO : effectuer la connection reelle.
			this->IsConnected = true;
		}
	}
	else
	{
		std::cout << "Client already connected" << std::endl;
	}
}

void	ClientConnection::Disconnect()
{
	std::cout << KCYN "Disconnection" KRESET<< std::endl;
	if (IsConnected)
	{
		IsConnected = false;
	}
	else
	{
		std::cout << "Client is not connected" << std::endl;
	}
}

void	ClientConnection::DisplayInfos()
{
	std::cout << KGRN "Connection infos:" KRESET << std::endl;
	std::cout << "TeamName: " << this->Settings.TeamName << std::endl;
	std::cout << "Port: " << this->Settings.Port << std::endl;
	std::cout << "HostName: " << this->Settings.HostName << std::endl;
}

void	ClientConnection::SetMockMode(bool b)
{
	this->IsInMockMode = b;
}