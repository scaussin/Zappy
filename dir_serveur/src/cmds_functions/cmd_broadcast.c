#include "../../includes/serveur.h"

int     on_start_cmd_broadcast(t_serveur *serv, t_client_entity *client_cur, char *param)
{
    char                    *msg;

    if (!regex_match(param, "^[a-zA-Z0-9 ']+\n$"))
    {
        printf(KMAG "Bad format to cmd [broadcast] "
                    "from sock %d\n" KRESET, client_cur->sock);
        return (-1);
    }

    // gfx msg
    asprintf(&msg, "pbc #%d %.*s\n",client_cur->sock, (int)strlen(param) - 1, param);
    push_gfx_msg(serv, msg);
    free(msg);
    return (0);
}

void    on_end_cmd_broadcast(t_serveur *serv, t_client_entity *client_cur, char *param)
{
    cmd_broadcast(serv, client_cur, param);
    write_buffer(&client_cur->buff_send, "ok\n", 3);
}

void	cmd_broadcast(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	(void)serv;
	(void)client_cur;

	t_player				*cur_player;
	int						provenance;
	char					*msg_client;
	t_client_entity			*list_clients;

	cur_player = &(client_cur->player);
	list_clients = serv->client_hdl.list_clients;
	while (list_clients)
	{
		if (list_clients->is_in_game == 1
			&& list_clients->is_gfx == 0
			&& list_clients->is_disconnecting == 0
			&& &(list_clients->player) != cur_player)
		{
			//calcul d'où provient le son
			provenance = provenance_son(serv->world_hdl.map_x, serv->world_hdl.map_y, cur_player->pos.x, cur_player->pos.y,
				list_clients->player.pos.x , list_clients->player.pos.y);
			provenance = provenance_with_dir(provenance,list_clients->player.dir);

			//asprintf(&msg_client, "message %d,%s, tu es en %d , %d\n, tu regardes vers %d",provenance, arg, list_clients->player.pos.x, list_clients->player.pos.y, list_clients->player.dir);
			asprintf(&msg_client, "message %d,%s",provenance, param);
			write_buffer(&list_clients->buff_send, msg_client, strlen(msg_client));
			free(msg_client);
			msg_client = NULL;
		}	
		list_clients = list_clients->next;
	}
}

double provenance_with_dir(int provenance, int dir)
{
	if (provenance == 0)
		return 0;
	provenance = (provenance + 2 * dir) % 8;
	if (provenance == 0)
		return 8;
    return provenance;
}


double distance(int sourcex, int sourcey, int i, int j)
{

    double a = (sourcex - i) * (sourcex - i) + (sourcey - j) * (sourcey - j);
    //printf("sourcex : %d  sourcey : %d  a: %d  b: %d,  dist:%lf\n",sourcex,sourcey,i,j,sqrt(a));
    return sqrt(a);
}

int orientation(int sourcex, int sourcey, int i, int j)
{
    if (abs(sourcex - i) > abs(sourcey - j))
    {
        if (sourcex > i)
            return 7;
        else
            return 3;
    }
    else if (abs(sourcey - j) > abs(sourcex - i))
    {
        if (sourcey > j)
            return 1;
        else
            return 5;
    }
    else if (abs(sourcey - j) == abs(sourcex - i))
    {
        if (sourcey >= j && sourcex >= i)               //son venant de haut droite
            return 8;
        else if (sourcey >= j && sourcex <= i)              //son venant de haut gauche
            return 2;
        else if (sourcey <= j && sourcex <= i)              //son venant de bas gauche
            return 4;
        else if (sourcey <= j && sourcex >= i)              //son venant de bas droite
            return 6;
    }
    return 0;
}

int provenance_son(int taillex, int tailley, int sourcex, int sourcey, int destx, int desty)
{
        if (sourcex == destx && sourcey == desty)
            return 0;
        int a = destx;
        int b = desty;

        double dist;
        double mindis = 2 * taillex * tailley;
        int orient = 0;

        //haut droite
        a = destx;
        b = desty;
        while (a <= 2 * taillex && b <= 2 * tailley)
        {

            if ((dist = distance(sourcex, sourcey, a, b))< mindis )
            {
                mindis = dist;
                orient = orientation(sourcex, sourcey, a, b);
            }
            a = a + taillex;
            b = b + tailley;
        }

        //haut
        a = destx;
        b = desty;
        while (b <= 2 * tailley)
        {

            if ((dist = distance(sourcex, sourcey, a, b))< mindis )
            {
                mindis = dist;
                orient = orientation(sourcex, sourcey, a, b);
            }
            b = b + tailley;
        }

        //haut gauche
        a = destx;
        b = desty;
        while (a >= -taillex && b <= 2 * tailley)
        {

            if ((dist = distance(sourcex, sourcey, a, b))< mindis )
            {
                mindis = dist;
                orient = orientation(sourcex, sourcey, a, b);
            }
            a = a - taillex;
            b = b + tailley;
        }

        //gauche
        a = destx;
        b = desty;
        while (a >= -taillex )
        {

            if ((dist = distance(sourcex, sourcey, a, b))< mindis )
            {
                mindis = dist;
                orient = orientation(sourcex, sourcey, a, b);
            }
            a = a - taillex;
        }

        //bas gauche
        a = destx;
        b = desty;
        while (a >= -taillex && b >= -tailley)
        {

            if ((dist = distance(sourcex, sourcey, a, b))< mindis )
            {
                mindis = dist;
                orient = orientation(sourcex, sourcey, a, b);
            }
             a = a - taillex;
            b = b - tailley;
        }

        //bas
        a = destx;
        b = desty;
        while (b >= -tailley)
        {

            if ((dist = distance(sourcex, sourcey, a, b))< mindis )
            {
                mindis = dist;
                orient = orientation(sourcex, sourcey, a, b);
            }
             b = b - tailley;
        }

        //bas droite
        a = destx;
        b = desty;
        while (a <= 2 * taillex && b >= -tailley)
        {

            if ((dist = distance(sourcex, sourcey, a, b))< mindis )
            {
                mindis = dist;
                orient = orientation(sourcex, sourcey, a, b);
            }
            a = a + taillex;
            b = b - tailley;
        }

        // droite
        a = destx;
        b = desty;
        while (a <= 2 * taillex )
        {

            if ((dist = distance(sourcex, sourcey, a, b))< mindis )
            {
                mindis = dist;
                orient = orientation(sourcex, sourcey, a, b);
            }
            a = a + taillex;
        }

     /*  printf("distance : %lf   ", mindis);
       printf("orientation : %d   ", orient);*/
       return (orient);
}
