#include "../../includes/serveur.h"

void	gfx_cmd_msz(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	(void)serv;
	(void)gfx_client;
	(void)param;
	char			*msg;

	asprintf(&msg, "msz %d %d\n", serv->world_hdl.map_x, serv->world_hdl.map_y);
	write_buffer(&gfx_client->buff_send, msg, strlen(msg));
	free(msg);
}