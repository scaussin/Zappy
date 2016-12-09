#include "../includes/Client.hpp"

int		main(int argc, char **argv)
{
	ClientController		*Client;

	Client = new ClientController();
	Client->OnInitialize(argc, argv);

	delete Client;
	return (0);
}
