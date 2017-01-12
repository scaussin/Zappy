#ifndef CLIENTCOMMUNICATION_HPP
# define CLIENTCOMMUNICATION_HPP

# include "Client.hpp"
#include <netdb.h>

class ClientCommunication
{
	public:
		ClientCommunication(std::string hostName, int port, ClientPlayer *player);
		~ClientCommunication();
		void					connect();
		void					disconnect();
		void					initFd();
		void					doSelect();
		void					checkFd();
		void					pullData();
		void					pushData();
		//void					displayInfos();

		bool					isConnected;
		bool					isAuthenticate;
		ClientPlayer			*player;
		ClientBuffer			bufferRecv;
		ClientBuffer			bufferSend;

	private:
		fd_set					*read_fs;
		fd_set					*write_fs;
		int						port;
		std::string				hostName;
		int						sock;
		struct protoent			*proto;
		struct sockaddr_in		sin;
};

#endif