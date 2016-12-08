#ifndef CLIENTCONNECTION_HPP
# define CLIENTCONNECTION_HPP

# include "Client.hpp"

class ClientConnection
{
	public:
		t_ConnectionSettings	Settings;
		bool					IsInMockMode; // allow false connection.


		ClientConnection();
		~ClientConnection();

		void	Connect();
		void	Disconnect();

		// to implement
		//	void	SendMessage(char *msg);
		//	void	ReceiveMessage();

		void	DisplayInfos();
		void	SetMockMode(bool b);


};

#endif