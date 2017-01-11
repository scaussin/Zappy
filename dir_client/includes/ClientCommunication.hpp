#ifndef CLIENTCOMMUNICATION_HPP
# define CLIENTCOMMUNICATION_HPP

# include "Client.hpp"

class ClientCommunication
{
	public:
		ClientCommunication(std::string hostName, int port, ClientPlayer *client);
		~ClientCommunication();
		void			connect();
		void			disconnect();
		void			pullServer();
		void			pushServer();
		void			doSelect();
		//void			displayInfos();
		
		bool			isConnected;
		bool			isAuthenticate;
		ClientPlayer	*client;

	private:
		int						port;
		std::string				hostName;
		int						sock;
		struct protoent			*proto;
		struct sockaddr_in		sin;
};

#endif