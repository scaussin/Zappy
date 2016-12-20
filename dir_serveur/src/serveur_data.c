#include "../includes/serveur.h"

void	init_data(t_serveur *serv)
{
	memset(serv, 0, sizeof(t_serveur));
}

void init_server(t_serveur *serv)
{
	SOCKET      sock;
	SOCKADDR_IN sin;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		exit_error("socket()");
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(serv->network.port);
	sin.sin_family = AF_INET;
	if(bind(sock,(SOCKADDR *)&sin, sizeof sin) < 0)
		exit_error("bind()");
	if(listen(sock, serv->network.max_clients) < 0)
		exit_error("listen()");
	serv->network.sock_endpoint = sock;
	serv->network.sock_max = sock;
	serv->client_hdl.nb_clients = 0;
}
