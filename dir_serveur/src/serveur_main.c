#include "../includes/serveur.h"

int main(int argc, char **argv)
{
  t_server_data server;
  fd_set        read_fs;

  // Parse arg and fill server data
  get_input(&(server.serv_settings), argc, argv);

  // Init server
  init_server(&server);

  // Boucle du jeux
  while (42)
  {
    int i;

    // Clear le set de fd ready pour lecture
    FD_ZERO(&read_fs);
    // Surveille l'entrée standard pour exit propre quand enter presse
    FD_SET(STDIN_FILENO, &read_fs);
    // Surveille le endpoint pour new connection
    FD_SET(server.sock_endpoint, &read_fs);
    // Surveille tous les client
    i = -1;
    while (++i < server.nb_clients)
      FD_SET(server.list_clients[i].sock, &read_fs);

    // BOUYAKA SELECT IS HERE !!!!!!!!!!
    if(select(server.last_sock + 1, &read_fs, NULL, NULL, NULL) < 0)
      exit_error("select()");

    // Exit when enter presse
    if (FD_ISSET(STDIN_FILENO, &read_fs))
      break;
    // Connect new client
    else if (FD_ISSET(server.sock_endpoint, &read_fs))
      new_client_connection(&server);
    // Check commands from clients
    else
      ckeck_all_clients_communication(&server, &read_fs);

    // Todo : exec les commandes clients
    manage_cmd_clients(&server);
  }

  // Close les socket clients
  close_all_connections(&server);
  return (0);
}
