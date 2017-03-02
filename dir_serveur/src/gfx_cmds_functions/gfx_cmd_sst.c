#include "../../includes/serveur.h"

/*
**	gfx command sst: asking for changing the world time unit.
*/

void	gfx_cmd_sst(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	(void)serv;
	(void)gfx_client;
	(void)param;
	char	*gfx_msg;
	int		new_t;
	char	*arg;
	float	old_t_unit;

	if (!regex_match(param, "^sst [0-9]+\n$"))
	{
		printf(KMAG "Bad format to gfx cmd [sst] "
					"from gfx client\n" KRESET);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
		return ;
	}
	errno = 0;
	arg = strchr(param, ' ') + 1;
	new_t = strtol(arg, NULL, 10);
	if (errno == ERANGE || errno == EINVAL)
	{
		printf(KMAG "Bad format to cmd [sst] "
				"from gfx client (bad integer format)\n" KRESET);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
		return ;
	}
	if (new_t > 1000)
	{
		printf(KMAG "Bad format to cmd [sst] "
				"from gfx client (max value = 1000)\n" KRESET);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
		return ;
	}
	old_t_unit = serv->world_hdl.t_unit;
	serv->world_hdl.t_unit = (float)(1.0 / new_t);
	refresh_times(serv, old_t_unit);
	asprintf(&gfx_msg, "sgt %f\n", serv->world_hdl.t_unit);
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
}