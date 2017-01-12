#ifndef CLIENTCOMMUNICATION_HPP
# define CLIENTCOMMUNICATION_HPP

# include "Client.hpp"

class ClientCommunication
{
	public:
		ClientCommunication(std::string hostName, int port, ClientPlayer *player);
		~ClientCommunication();
		void					connectToServer();
		void					disconnect();
		void					initFd();
		void					doSelect();
		void					checkFd();
		void					manageRecv();
		int						pullData();
		void					pushData();
		void					clientAuthentication();
		void					replaceEnd(std::string &str);
		void					parseTeamSlots(std::string msg, int *teamSlots);
		void					parseWorldSize(std::string msg, int *worldSizeX, int *worldSizeY);
		//void					displayInfos();

		bool					isConnected;
		bool					isAuthenticate;
		ClientPlayer			*player;

		ClientBuffer			bufferRecv;
		ClientBuffer			bufferSend;
/*
typedef struct	sMatchValue
{
	std::regex regex;
	eValueType type;
	sMatchValue(std::regex reg, eValueType value) : regex(reg), type(value)
	{
	}
}				tMatchValue;

typedef struct	sCheckInstr
		{
			eInstrType type;
			void (Parser::*func)(tToken *);
			sCheckInstr(eInstrType type, void (Parser::*funct)(tToken *)) :type(type), func(funct)
			{
			}
		}

		std::vector<tCheckValue> _checkValue;*/


	private:
		int						stateProcessAuth;
		fd_set					read_fs;
		fd_set					write_fs;
		int						port;
		std::string				hostName;
		int						sock;
		struct protoent			*proto;
		struct sockaddr_in		sin;
};

#endif