#ifndef CLIENTCOMMUNICATION_HPP
# define CLIENTCOMMUNICATION_HPP

# include "client.hpp"

class ClientCommunication
{
	public:
		ClientCommunication(string hostName, int port, ClientPlayer *player, ClientIa *_ia);
		~ClientCommunication();
		void					connectToServer();
		void					disconnect();
		void					initFd();
		void					doSelect();
		int						checkFd();
		void					manageRecv();
		int						pullData();
		void					pushData();
		void					clientAuthentication();
		void					replaceEnd(string &str);
		void					parseTeamSlots(string msg, int *teamSlots);
		void					parseWorldSize(string msg, int *worldSizeX, int *worldSizeY);
		//void					displayInfos();

		bool					isConnected;
		bool					isAuthenticated;
		ClientPlayer			*player;

		ClientBuffer			bufferRecv;
		ClientBuffer			bufferSend;

	private:
		ClientIa				*ia;
		int						stateProcessAuth;
		fd_set					read_fs;
		fd_set					write_fs;
		string					hostName;
		int						port;
		int						sock;
		struct protoent			*proto;
		struct sockaddr_in		sin;
};

#endif