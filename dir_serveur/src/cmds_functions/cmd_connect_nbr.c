#include "../../includes/serveur.h"

/*
**	La commande connect_nbr renvoie le nombre de connections autorisées et non
**	autorisées pour cette famille.
**
**	connaitre le nb de connections	| connect_nbr	| 0/t	|	valeur
**	non utilisée par l’équipe		|
*/
void	cmd_connect_nbr(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	(void)serv;
	(void)param;
	char *client_msg;

	if (!regex_match(param, "^connect_nbr\n$"))
	{
		printf(KMAG "Bad format to cmd [connect_nbr]"
					"from sock %d\n" KRESET, client_cur->sock);
		return ;
	}
	asprintf(&client_msg, "%d\n", client_cur->team->available_slots);
	write_buffer(&client_cur->buff_send, client_msg, strlen(client_msg));
	free(client_msg);
}
