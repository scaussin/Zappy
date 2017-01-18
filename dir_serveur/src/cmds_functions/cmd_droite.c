#include "../../includes/serveur.h"

void	cmd_droite(struct s_serveur *serv, struct s_client_entity *client_cur, char *param) /* typedef ?? */
{
	(void) param;
	t_player	*cur_player;
	char		*msg;

	cur_player = &(client_cur->player);
	cur_player->dir += 1;
	if (cur_player->dir > 3)
	{
		cur_player->dir = 0;
	}

	// player client response.
	write_buffer(&client_cur->buff_send, "ok\n", 3);
	// gfx msg : "ppo #n X Y O\n" 
	asprintf(&msg, "ppo #%d %d %d %d\n",
		client_cur->sock,
		cur_player->pos.x,
		cur_player->pos.y,
		cur_player->dir + 1);
	push_gfx_msg(serv, msg);
	free(msg);
}
