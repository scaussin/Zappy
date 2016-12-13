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
			try
			{
				ConnectToServer();
				this->IsConnected = true;
			}
			catch (CustomException &e)
			{
				std::cout << KRED << e.what() << KRESET << std::endl;
			}
		}
	}
	else
	{
		std::cout << "Client already connected" << std::endl;
	}
}

void	ClientConnection::ConnectToServer()
{
	if (Settings.HostName.compare("localhost") == 0)
	{
		Settings.HostName = "127.0.0.1";
	}
	this->proto = getprotobyname("tcp");
	if (!this->proto)
		throw (CustomException("getprotobyname() error"));
	this->sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	this->sin.sin_family = AF_INET;
	this->sin.sin_port = htons(Settings.Port);
	this->sin.sin_addr.s_addr = inet_addr(Settings.HostName.c_str());
	if (connect(this->sock, (const struct sockaddr *)&(this->sin), sizeof(this->sin)) == -1)
	{
		throw (CustomException("connect() error : cannot connect to hostname"));
		exit (-1);
	}
	std::cout << KGRN "SUCCESS - Connected to server.\n" KRESET << std::endl;
	std::cout << "Port: " << Settings.Port << std::endl;
	std::cout << "Client socket: " << Settings.Port << std::endl;
	std::cout << "Client socket: %d\n" << this->sock << std::endl;
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