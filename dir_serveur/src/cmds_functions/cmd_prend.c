#include "../../includes/serveur.h"

void	cmd_prend(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	(void)serv;
	(void)client_cur;
	(void)param;

	write_buffer(&client_cur->buff_send, "ok\n", 3);
}
