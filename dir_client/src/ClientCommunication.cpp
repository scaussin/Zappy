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
}

ClientCommunication::~ClientCommunication()
{}

void	ClientCommunication::initFd()
{
	FD_ZERO(read_fs);
	FD_ZERO(write_fs);
	
	// Surveille l'entrée standard pour exit propre quand enter presse
	FD_SET(STDIN_FILENO, read_fs);

	/*
		TODO: if disconnecting
	*/

	FD_SET(sock, read_fs);
	if (bufferSend.getLen() > 0)
		FD_SET(sock, write_fs);
}

void	ClientCommunication::doSelect()
{
	if (select(sock + 1, read_fs, write_fs, NULL, NULL) < 0)
		throw (CustomException("select() error"));
}

void	ClientCommunication::checkFd()
{
	if (FD_ISSET(STDIN_FILENO, read_fs))
		disconnect();
	if (FD_ISSET(sock, read_fs))
		pullData();
	if (FD_ISSET(sock, write_fs))
		pushData();
}

void	ClientCommunication::connect()
{
	struct hostent		*host;
	
	if (!isConnected)
	{
		//std::cout << KCYN "- Connection attempt... -" KRESET << std::endl;
		if ((host = gethostbyname(hostName)))
			hostName = inet_ntoa(*((struct in_addr **)(host->h_addr_list))[0]);

		proto = getprotobyname("tcp");
		if (!proto)
			throw (CustomException("getprotobyname() error"));
		sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		sin.sin_addr.s_addr = inet_addr(hostName.c_str());
		if (connect(sock, (const struct sockaddr *)&(sin), sizeof(sin)) == -1)
		{
			throw (CustomException("connect() error: cannot connect to hostname"));
			exit (-1);
		}
	}
	else
		std::cout << "Client already connected" << std::endl;
}
/*
void	ClientCommunication::dialogStart()
{
	struct timeval		timeout;
	fd_set				readSet;
	int					select_ret;

	// First batch of data reception
	std::cout << KCYN "- Awaiting server response... -" KRESET << std::endl;
	timeout.tv_sec = 3; // seconds.
	timeout.tv_usec = 0; // milliseconds.
	FD_ZERO(&readSet);
	FD_SET(sock, &readSet);
	select_ret = select(sock + 1, &readSet, NULL, NULL, &timeout);
	if (select_ret == -1)
	{
		perror("Select");
		throw (CustomException("Select error"));
	}
	if (FD_ISSET(sock, &readSet))
	{
		std::string		receivedString = ReceiveMessage();
		std::cout << KBLU << "Received: " << KRESET << receivedString << std::endl;
		// Is it the right server ?
		if (receivedString.compare("BIENVENUE\n") == 0)
		{
			std::cout << KYEL << "Sending team name: " << KRESET << Settings.TeamName << std::endl;
			SendMessage(Settings.TeamName + "\n");

			// Receiving first datas.
			receivedString = ReceiveMessage();
			timeou
			std::cout << KBLU << "Received: " << KRESET << receivedString << std::endl;

			// Recuperating : <slot_number>\n<x_position> <y_position>\n
			char	*splittedString;
			// getting slot number.
			if (!(splittedString = strtok((char *)receivedString.c_str(), " \n")))
			{
				throw (CustomException("Error while getting slot number"));
			}
			TeamSlots = strtol(splittedString, NULL, 10);
			// getting X start position.
			if (!(splittedString = strtok(NULL, " \n")))
			{
				throw (CustomException("Error while getting x starting position"));
			}
			Startx = strtol(splittedString, NULL, 10);
			// getting Y start position.
			if (!(splittedString = strtok(NULL, " \n")))
			{
				throw (CustomException("Error while getting y starting position"));
			}
			Starty = strtol(splittedString, NULL, 10);
			std::cout << "Nb of slots received: " << TeamSlots << std::endl;
			if (TeamSlots <= 0)
			{
				throw (CustomException("No slot available in team or team does not exist."));
			}
		}
		else
		{
			throw (CustomException("Wrong starting message received (incorrect server?)"));
		}
	}
	else
	{
		throw (CustomException("Connection timed out (the server did not answer). Closing client..."));
	}
	std::cout << KGRN "First dialog with server successful: Zappy server recognized" KRESET << std::endl;
}*/

/*
// Recuperating : <slot_number>\n<x_position> <y_position>\n
	char	*splittedString;
	// getting slot number.
	if (!(splittedString = strtok((char *)receivedString.c_str(), " \n")))
	{
		throw (CustomException("Error while getting slot number"));
	}
	TeamSlots = strtol(splittedString, NULL, 10);
	// getting X start position.
	if (!(splittedString = strtok(NULL, " \n")))
	{
		throw (CustomException("Error while getting x starting position"));
	}
	Startx = strtol(splittedString, NULL, 10);
	// getting Y start position.
	if (!(splittedString = strtok(NULL, " \n")))
	{
		throw (CustomException("Error while getting y starting position"));
	}
	Starty = strtol(splittedString, NULL, 10);
	std::cout << "Nb of slots received: " << TeamSlots << std::endl;
	if (TeamSlots <= 0)
	{
		throw (CustomException("No slot available in team or team does not exist."));
	}
*/

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
		retSend = send(sock, buffTmp.c_str(), bufferSend.getLen, 0);
		if (retSend == -1 && (errno == EAGAIN || errno == EINTR))
			continue;
		else
			break ;
	}
	std::cout << "send: " << buffTmp.substr(0, retSend) << std::endl;
	if (retSend == -1)
		throw (CustomException("send error"));
	if (retSend > 0)
		bufferSend.erase(retSend);
}

/*
** receive data server
*/
void	ClientCommunication::pullData()
{
	int		retRecv;
	int		sizeRead;
	char	*buffRecv;

	sizeRead = BUFF_SIZE - bufferRecv.getLen;
	if (sizeRead == 0)
	{
		throw (CustomException("buffer full"));
		return ;
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
	buffRecv[retRecv] = 0;
	std::cout << "recv: " << buffRecv << std::endl;
	delete[] buffRecv;
}

/*
void	ClientCommunication::displayInfos()
{
	std::cout << KGRN "Connection infos:" KRESET << std::endl;
	std::cout << "TeamName: " << Settings.TeamName << std::endl;
	std::cout << "Port: " << Settings.Port << std::endl;
	std::cout << "HostName: " << Settings.HostName << std::endl;
}*/
