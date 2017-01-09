#include "../includes/serveur.h"

/*
**	Check the reception of the client.
**	If received TEAM NAME, client goes into the game.
**	If received GRAPHIC, client is the gfx.
*/

void	client_recognition(t_serveur *serv, t_client_entity *client)
{
	char			*buff;
	char			*str_to_send;
	t_team_entity	*team;

	if ((buff = read_buffer(&client->buff_recv)))
	{
		if (strcmp(buff, "GRAPHIC\n") == 0)
		{
			client->is_gfx = 1;
			serv->client_hdl.gfx_client = client;
			// sending world size;
			asprintf(&str_to_send, "msz %d %d\n", serv->world_hdl.map_x, serv->world_hdl.map_y);
			write_buffer(&client->buff_send, str_to_send, strlen(str_to_send));
			free(str_to_send);
			printf(KGRN "GFX client recognized\n" KRESET);
		}
		else
		{
			if (!(team = get_team_by_name(serv, buff)))
			{
				printf("Get_team() failed\n");
				write_buffer(&client->buff_send, "UNKNOWN TEAM\n", 10);
				disconnect_client(client->sock);
				free(buff);
				return ;
			}
			// sending first datas; <nb-client>\n<x><y>\n
			client->is_in_game = 1;
			client->buff_recv.len = 0;
			printf(KGRN "Player client recognized\n" KRESET);
			asprintf(&str_to_send, "%d\n%d %d\n", team->available_slots, serv->world_hdl.map_x, serv->world_hdl.map_y);
			write_buffer(&client->buff_send, str_to_send, strlen(str_to_send));
		}
	}
}