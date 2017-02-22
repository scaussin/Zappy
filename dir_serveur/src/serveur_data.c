#include "../includes/serveur.h"

void	init_data(t_serveur *serv)
{
	memset(serv, 0, sizeof(t_serveur));
}

void	fill_team_info(t_serveur *serv)
{
	int	i;

	i = -1;
	while (++i < serv->team_hdl.nb_teams)
	{
		serv->team_hdl.array_teams[i].available_slots
			= serv->team_hdl.nb_teams_slots;
	}
}

void	init_serveur(t_serveur *serv)
{
	SOCKET		sock;
	SOCKADDR_IN	sin;
	int			optval;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		exit_error("socket()");
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(serv->network.port);
	sin.sin_family = AF_INET;
	optval = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
		exit_error("setsockopt");
	if (bind(sock,(SOCKADDR *)&sin, sizeof sin) < 0)
		exit_error("bind()");
	if (listen(sock, serv->network.max_clients) < 0)
		exit_error("listen()");

	// Team info
	fill_team_info(serv);

	// Network info
	serv->network.sock_serveur = sock;
	serv->network.sock_max = sock;
	serv->network.read_fs = (fd_set *)s_malloc((sizeof(fd_set)));
	serv->network.write_fs = (fd_set *)s_malloc((sizeof(fd_set)));

	// Client infos
	serv->client_hdl.nb_clients = 0;
	serv->client_hdl.list_clients = NULL;
	serv->client_hdl.gfx_client = NULL;

	// set worlds default values.
	serv->loop_nb = 0;
	serv->victory_reached = 0;
	serv->world_hdl.nb_of_eggs = 0;
	serv->world_hdl.nb_of_incantations = 0;
	serv->world_hdl.ongoing_events = NULL;

	// Set default games rules/mechanics
	serv->settings_hdl.is_pickup_instant = 1;
	serv->settings_hdl.can_interrupt_incantation = 0;
	serv->settings_hdl.are_teams_growing = 1;
}
