#include "../../includes/serveur.h"

void	cmd_avance(struct s_serveur *serv, struct s_client_entity *client_cur, char *param) /* typedef ?? */
{
	(void) param;
	(void) serv;
	write_buffer(&client_cur->buff_send, "OK\n", 3);
	client_cur->list_cmds->time = 0;
}
