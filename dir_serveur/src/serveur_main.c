#include "../includes/serveur.h"

int main(int argc, char **argv)
{
	t_serveur			serv;
	
	// Init datas.
	init_data(&serv);

	// Parse arg and fill server data
	get_input(&serv, argc, argv);

	// Init Terrain from get_input datas
	init_terrain(&serv);

	// Init server
	init_serveur(&serv);

	// Link commands and corresponding functions in an array.
	init_cmd_match_table(&serv); // -> cmd_clients_init.c

	// Boucle du jeux
	main_loop(&serv);

	// Close les connections / free allocated memory.
	close_all_connections(&serv);
	free(serv.cmd_hdl.cmd_match_table);

	return (0);
}
