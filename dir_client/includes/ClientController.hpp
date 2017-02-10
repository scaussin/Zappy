#ifndef CLIENTCONTROLLER_HPP
# define CLIENTCONTROLLER_HPP

# include "client.hpp"

/*
**	Main instance of the client. Controls every other client class;
*/
class ClientController
{
	public:
		ClientController(int argc, char **argv);
		~ClientController();

		void					startGame();
		void					mainLoop();

		ClientInputHandler		*inputHandler;
		ClientCommunication		*communication;	// will handle every server related discussion.
		ClientPlayer			*player;
		ClientIa				*ia;

	private:
};

#endif