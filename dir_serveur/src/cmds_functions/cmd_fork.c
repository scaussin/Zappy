#include "../../includes/serveur.h"

/*
**	Fork will put the player in egg laying status. When the egg is layed(after 42/t),
**	the player can continue his life. The egg will lay after 600/t, see game_player_events.c
**	for what happen when the player finished laying the egg.
*/

void	cmd_fork(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	(void)param;
	t_player		*cur_player;
	char			*client_msg;
	char			*gfx_msg;

	if (!regex_match(param, "^fork\n$"))
	{
		printf(KMAG "Bad format to cmd [fork] "
					"from sock %d\n" KRESET, client_cur->sock);
		return ;
	}
	cur_player = &client_cur->player;		

	// Set player time of egg laying. -> 42/t default
	client_cur->player.is_laying_egg = 1;
	get_time(&client_cur->player.egg_layed_time);
	add_nsec_to_timespec(&client_cur->player.egg_layed_time,
		EGG_LAYING_TIME * serv->world_hdl.t_unit * 1000000000);

	client_msg = strdup("ok\n");
	write_buffer(&client_cur->buff_send, client_msg, strlen(client_msg));

	// gfx msg : "pfk #n\n"
	asprintf(&gfx_msg, "pfk #%d\n", client_cur->sock);
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
}
