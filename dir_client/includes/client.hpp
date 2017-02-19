#ifndef CLIENT_HPP
# define CLIENT_HPP

// General includes
# include <unistd.h>
# include <stdlib.h>
# include <iostream>
# include <sstream>
# include <string>
# include <stdio.h>
# include <regex>
# include <errno.h>
# include <algorithm>
# include <vector>
# include <list>
# include <map>

// Connection includes.
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <fcntl.h>
# include <sys/time.h>
# include <ctime>


# define BUFF_SIZE 8192
# define END "\n"
# define LEN_END 1
# define LEVEL_MAX 8

using namespace std;
/*
// Connection related structure.
typedef struct			s_CommunicationSettings
{
	string				TeamName;
	int						Port;
	string				HostName;


}							t_CommunicationSettings;*/

// Class definitions.
class ClientInputHandler; // handle and check args.
class ClientBuffer;
class ClientCommunication; // communicate with the server.
class ClientPlayer; // the actual game player.
class ClientController; // main controller.
class ClientIa;


// Class files includes.
# include "CustomException.hpp"
# include "ClientInputHandler.hpp"
# include "ClientBuffer.hpp"
# include "ClientCommunication.hpp"
# include "ClientPlayer.hpp"
# include "ClientController.hpp"
# include "ClientIa.hpp"

vector<string>	strSplit(string str, char c);

// color in text;
# define KNRM  "\x1B[0m"
# define KRED  "\x1B[31m"
# define KGRN  "\x1B[32m"
# define KYEL  "\x1B[33m"
# define KBLU  "\x1B[34m"
# define KMAG  "\x1B[35m"
# define KCYN  "\x1B[36m"
# define KWHT  "\x1B[37m"
# define KRESET "\x1B[0m"

# define MIN_FOOD_1 5000
# define MIN_FOOD_2 5000
//# define MIN_FOOD_BEFORE_INCANTATION 5000

# define N_TO_EAT_1 50
# define N_TO_EAT_2 50
//# define N_TO_EAT_BEFORE_INCANTATION 50
# define FOOD "nourriture"
# define SUCCESS "ok\n"

# define MODULO_BROADCAST 8

#endif
