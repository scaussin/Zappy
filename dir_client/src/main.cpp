#include "../includes/Client.hpp"

int		main(int argc, char **argv)
{
	ClientController	client(argc, argv);

	if (client.communication->isConnected)
	{
		client.mainLoop();	
	}

	return (0);
}
