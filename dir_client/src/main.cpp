#include "client.hpp"

int		main(int argc, char **argv)
{
	ClientController	client(argc, argv);
	 srand (time(NULL));

	if (client.communication && client.communication->isConnected)
	{
		client.mainLoop();
	}
	return (0);
}
