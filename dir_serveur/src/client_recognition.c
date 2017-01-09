#include "../includes/serveur.h"

/*
**	Check the reception of the client.
**	If received TEAM NAME, client goes into the game.
**	If received GRAPHIC, client is the gfx.
*/

int		client_recognition(t_serveur *serv, t_client_entity *client)
{
	char			*cmd;
	char			*str_to_send;
	t_team_entity	*team;

	if ((cmd = get_first_cmd(&client->buff_recv)))
	{
		printf(KGRN "okok\n" KRESET);
		if (strcmp(cmd, "GRAPHIC\n") == 0)
		{
			if (!serv->client_hdl.gfx_client)
			{
				serv->client_hdl.gfx_client = client;
				client->is_gfx = 1;
				// sending world size;
				asprintf(&str_to_send, "msz %d %d\n", serv->world_hdl.map_x, serv->world_hdl.map_y);
				write_buffer(&client->buff_send, str_to_send, strlen(str_to_send));
				free(str_to_send);
				printf(KGRN "GFX client recognized\n" KRESET);
				return (0);
			}
			else
			{
				printf(KRED "GFX client already set\n" KRESET);
				disconnect_client(client->sock);
				remove_client(serv, client);
				return (-1);
			}
		}
		else
		{
			if (!(team = get_team_by_name(serv, cmd)))
			{
				printf(KRED "Get_team() failed\n" KRESET);
				write_buffer(&client->buff_send, "UNKNOWN TEAM\n", 13);
				disconnect_client(client->sock);
				remove_client(serv, client);
				return (-1);
			}
			// sending first datas; <nb-client>\n<x><y>\n
			client->is_in_game = 1;
			client->team = team;
			printf(KGRN "Player client recognized\n" KRESET);
			asprintf(&str_to_send, "%d\n%d %d\n", team->available_slots, serv->world_hdl.map_x, serv->world_hdl.map_y);
			write_buffer(&client->buff_send, str_to_send, strlen(str_to_send));
			free(str_to_send);
			// one slot now taken in team.
			team->available_slots -= 1;
			return (0);
		}	
		free(cmd);
	}
	printf(KGRN "elseokok\n" KRESET);
	return (0);
}