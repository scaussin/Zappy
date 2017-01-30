#include "../../includes/serveur.h"

void	gfx_cmd_tna(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	(void)gfx_client;
	(void)param;
	int		x;
	char	*msg;

	x = 0;
	while (x < serv->team_hdl.nb_teams)
	{
		asprintf(&msg, "tna %s\n", serv->team_hdl.array_teams[x].name);
		push_gfx_msg(serv, msg);
		free(msg);
		x++;
	}
}