#ifndef CLIENTCONTROLLER_HPP
# define CLIENTCONTROLLER_HPP

# include "Client.hpp"

/*
**	Main instance of the client. Control every other client class;
*/
class ClientController
{
	public:
		ClientConnection	ClientConnection_obj;

		ClientController();
		~ClientController();


	
};

#endif