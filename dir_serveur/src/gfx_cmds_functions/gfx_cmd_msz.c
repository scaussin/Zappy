#include "../../includes/serveur.h"

/*
**	gfx command msz: asking for the world size.
*/

void	gfx_cmd_msz(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	(void)serv;
	(void)gfx_client;
	(void)param;
	char			*msg;

	if (!regex_match(param, "^msz\n$"))
	{
		printf(KMAG "Bad format to gfx cmd [msz] "
					"from gfx client\n" KRESET);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
		return ;
	}
	asprintf(&msg, "msz %d %d\n", serv->world_hdl.map_x, serv->world_hdl.map_y);
	write_buffer(&gfx_client->buff_send, msg, strlen(msg));
	free(msg);
}
