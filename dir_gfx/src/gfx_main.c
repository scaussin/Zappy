#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
** We take the beginning of argv[0] until the last '/' and
** append the path to our main scene to open unity.
*/

int		main(int argc, char **argv)
{
  char *command;
  char *path;
  int i;

  i = 0;
  if (argc == 1 && argv[0])
    {
      path = strrchr(argv[0], '/');
      *path = '\0';
      asprintf(&command, "open %s/gfx_unity_project/Assets/Scenes/gfx.unity", argv[0]);
      system(command);
      free(command);
    }
  return (0);
}
