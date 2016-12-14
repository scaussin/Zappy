#include "../includes/serveur.h"

void exit_error(char *error_log)
{
  perror(error_log);
  exit(errno);
}
