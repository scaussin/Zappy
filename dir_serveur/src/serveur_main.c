#include "../includes/serveur.h"

int main(int argc, char **argv)
{
	t_serveur	serv;
	t_match_lexer *match_lexer;

	// Init data
	init_data(&serv);
	// Parse arg and fill server data
	get_input(&serv, argc, argv);
	// Init server
	init_serveur(&serv);

	match_lexer = init_match_lexer();
	// Boucle du jeux
	main_loop(&serv, match_lexer);

	// Close les connections
	close_all_connections(&serv);
	free(match_lexer);

	return (0);
}
