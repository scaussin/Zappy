#include "../includes/Client.hpp"

//------------------------------------------------------------------------------//
//																				//
//		connection and communication with the server							//
//																				//
//																				//
//------------------------------------------------------------------------------//

ClientCommunication::ClientCommunication(std::string _hostName, int _port, ClientPlayer *_player) : hostName(_hostName), port(_port), player(_player)
{
	isConnected = false;
	isAuthenticate = false;
	sock = -1;
	stateProcessAuth = 0;
}

ClientCommunication::~ClientCommunication()
{}

void	ClientCommunication::initFd()
{
	FD_ZERO(&read_fs);
	FD_ZERO(&write_fs);
	
	// Surveille l'entrée standard pour exit propre quand enter presse
	FD_SET(STDIN_FILENO, &read_fs);

	/*
		TODO: if disconnecting
	*/

	FD_SET(sock, &read_fs);
	if (bufferSend.getLen() > 0)
		FD_SET(sock, &write_fs);
}

void	ClientCommunication::doSelect()
{
	if (select(sock + 1, &read_fs, &write_fs, NULL, NULL) < 0)
		throw (CustomException("select() error"));
}

int		ClientCommunication::checkFd()
{
	if (FD_ISSET(STDIN_FILENO, &read_fs))
	{
		disconnect();
		return (-1);
	}
	if (FD_ISSET(sock, &read_fs))
	{
		if (pullData() == 0)//read from server
		{
			disconnect();
			return (-1);
		}
	}
	if (FD_ISSET(sock, &write_fs))
		pushData();//write to server
	return (1);
}

void	ClientCommunication::manageRecv()
{
	if (bufferRecv.getLen() == 0)
		return ;
	if (isAuthenticate)
	{
		std::string msg = bufferRecv.getFirstMsg();
		if (msg.compare("mort\n") == 0)
		{
			//TODO ;)
		}
		else
			player->receiveCallbackCommande(msg);
	}
	else
		clientAuthentication();
}

void	ClientCommunication::clientAuthentication()
{
	if (stateProcessAuth == 0) // BIENVENUE/n
	{
		std::string msg = bufferRecv.getFirstMsg();
		if (msg.compare("BIENVENUE\n") == 0)
		{
			bufferSend.pushMsg(player->teamName + "\n");
			stateProcessAuth = 1;
		}
	}
	if (stateProcessAuth == 1 && bufferRecv.getLen() > 0) // <slot_number>\n
	{		
		try
		{
			std::string msg = bufferRecv.getFirstMsg();
			parseTeamSlots(msg, &player->teamSlots);
			stateProcessAuth = 2;
		}
		catch (CustomException &e)
		{
			std::cout << KRED << e.what() << KRESET << std::endl;
		}
	}
	if (stateProcessAuth == 2 && bufferRecv.getLen() > 0) // <x_position> <y_position>\n
	{
		try
		{
			std::string msg = bufferRecv.getFirstMsg();
			parseWorldSize(msg, &player->worldSizeX, &player->worldSizeY);
			player->bufferSend = &bufferSend;
			std::cout << KGRN << "client authenticated" << KRESET << std::endl;
			player->printStat();
			isAuthenticate = true;

			player->avance();
			player->avance();
			player->droite();
			player->avance();
			player->gauche();
			player->avance();
			player->voir();
		}
		catch (CustomException &e)
		{
			std::cout << KRED << e.what() << KRESET << std::endl;
		}
	}
}

void	ClientCommunication::replaceEnd(std::string &str)
{
	size_t start_pos;
	while ((start_pos = str.find(END)) != std::string::npos)
	{
		str.replace(start_pos, LEN_END, "*");
	}
}

void	ClientCommunication::connectToServer()
{
	struct hostent		*host;
	
	if (!isConnected)
	{
		//std::cout << KCYN "- Connection attempt... -" KRESET << std::endl;
		if ((host = gethostbyname(hostName.c_str())))
			hostName = inet_ntoa(*((struct in_addr **)(host->h_addr_list))[0]);

		proto = getprotobyname("tcp");
		if (!proto)
			throw (CustomException("getprotobyname() error"));
		sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		sin.sin_addr.s_addr = inet_addr(hostName.c_str());
		if (connect(sock, (const struct sockaddr *)&(sin), sizeof(sin)) == -1)
			throw (CustomException("connect() error: cannot connect to hostname"));
		isConnected = true;
	}
	else
		std::cout << "Client already connected" << std::endl;
}

/*
** parse : <slot_number>\n<x_position> <y_position>\n
** TODO: check error
*/
void	ClientCommunication::parseTeamSlots(std::string msg, int *teamSlots)
{
	std::regex expression("^([0-9]+)\n$");
	std::smatch matches;

	if(std::regex_search(msg, matches, expression))
	{
		*teamSlots = strtol(matches[0].str().c_str(), NULL, 10);
	} 
	else
		throw (CustomException("error parse team slots"));
}

void	ClientCommunication::parseWorldSize(std::string msg, int *worldSizeX, int *worldSizeY)
{
	std::regex expression("^([0-9]+) ([0-9]+)\n$");
	std::smatch matches;

	if(std::regex_search(msg, matches, expression))
	{
		*worldSizeX = strtol(matches[1].str().c_str(), NULL, 10);
		*worldSizeY = strtol(matches[2].str().c_str(), NULL, 10);
	} 
	else
		throw (CustomException("client auth fail"));
}

void	ClientCommunication::disconnect()
{
	if (isConnected)
	{
		isConnected = false;
		close(sock);
		std::cout << KCYN "Disconnected" KRESET<< std::endl;
	}
	else
		std::cout << "Client not connected" << std::endl;
}

/*
** send data server
*/
void	ClientCommunication::pushData()
{
	std::string buffTmp;
	int			retSend;

	buffTmp = bufferSend.getBuffer();
	while (1)
	{
		retSend = send(sock, buffTmp.c_str(), bufferSend.getLen(), 0);
		if (retSend == -1 && (errno == EAGAIN || errno == EINTR))
			continue;
		else
			break ;
	}
	replaceEnd(buffTmp = buffTmp.substr(0, retSend));
	std::cout << KYEL << " Sending to server: "<< KRESET << "["<< buffTmp << "]" << std::endl;
	if (retSend == -1)
		throw (CustomException("send error"));
	if (retSend > 0)
		bufferSend.erase(retSend);
}

/*
** receive data server
*/
int		ClientCommunication::pullData()
{
	int		retRecv;
	int		sizeRead;
	char	*buffRecv;

	sizeRead = BUFF_SIZE - bufferRecv.getLen();
	if (sizeRead == 0)
	{
		throw (CustomException("buffer full")); //not catched
		return (-1);
	}
	buffRecv = new char[sizeRead];
	while (1)
	{
		retRecv = recv(sock, buffRecv, sizeRead, 0);
		if (retRecv == -1 && (errno == EAGAIN || errno == EINTR))
			continue;
		else
			break ;
	}	
	if (retRecv == -1)
		throw (CustomException("recv error"));
	else
		bufferRecv.pushBuffer(buffRecv, retRecv);
	std::string recvStr(buffRecv, retRecv);
	replaceEnd(recvStr);
	std::cout << KBLU << " Receiving to server: "<< KRESET << "["<< recvStr << "] len: " << retRecv << std::endl;
	delete[] buffRecv;
	return (retRecv);
}

/*
void	ClientCommunication::displayInfos()
{
	std::cout << KGRN "Connection infos:" KRESET << std::endl;
	std::cout << "TeamName: " << Settings.TeamName << std::endl;
	std::cout << "Port: " << Settings.Port << std::endl;
	std::cout << "HostName: " << Settings.HostName << std::endl;
}*/
