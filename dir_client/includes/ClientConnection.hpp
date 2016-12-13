#ifndef CLIENTCONNECTION_HPP
# define CLIENTCONNECTION_HPP

# include "Client.hpp"

class ClientConnection
{
	public:
		t_ConnectionSettings	Settings;
		bool					IsInMockMode; // allow false connection.
		bool					IsConnected; // default false;

		

		ClientConnection();
		~ClientConnection();

		void	Connect();
		void	ConnectToServer();
		void	Disconnect();

		// to implement
		//	void	SendMessage(char *msg);
		//	void	ReceiveMessage();

		void	DisplayInfos();
		void	SetMockMode(bool b);

	private:
		// Connection variables
		int						sock;
		struct protoent			*proto;
		struct sockaddr_in		sin;

};

#endif