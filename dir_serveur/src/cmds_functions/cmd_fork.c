#include "../../includes/serveur.h"

/*
**	Fork will put the player in egg laying status. When the egg is layed(after 42/t),
**	the player can continue his life. The egg will lay after 600/t, see game_player_events.c
**	for what happen when the player finished laying the egg.
*/

int		on_start_cmd_fork(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	char *gfx_msg;

	if (param)
	{
		printf(KMAG "Bad format to cmd [fork] "
					"from sock %d\n" KRESET, client_cur->sock);
		return (-1);
	}
	printf(KGRN "[Serveur]: player #%d laying egg ... \n" KRESET, client_cur->sock);
	cmd_fork(serv, client_cur, param);
	// gfx msg : "pfk #n\n"
	asprintf(&gfx_msg, "pfk #%d\n", client_cur->sock);
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
	return (0);
}

void	on_end_cmd_fork(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	(void)			param;
	char			*gfx_msg;

	// the egg will be created and added to the event check.
	add_new_egg(serv, client_cur);
	write_buffer(&client_cur->buff_send, "ok\n", 3);
	// send gfx: "enw #e #n X Y\n"
	asprintf(&gfx_msg, "enw #%d\n", client_cur->sock);
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
}

void	cmd_fork(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	(void)			param;
	(void)			serv;
	(void)			client_cur;
}
