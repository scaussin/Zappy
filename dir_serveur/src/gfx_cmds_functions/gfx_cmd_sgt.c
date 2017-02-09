#include "../../includes/serveur.h"

/*
**	gfx command sgt: asking for the world's time unit == 1 / t.
*/

void	gfx_cmd_sgt(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	(void)serv;
	(void)gfx_client;
	(void)param;
	char *gfx_msg;

	if (!regex_match(param, "^sgt\n$"))
	{
		printf(KMAG "Bad format to gfx cmd [sgt] "
					"from gfx client\n" KRESET);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
		return ;
	}
	asprintf(&gfx_msg, "sgt %f\n", serv->world_hdl.t_unit);
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
}