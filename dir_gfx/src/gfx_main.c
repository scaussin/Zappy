#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int		main(void)
{
  char *command;
  
  asprintf(&command, "open gfx_unity_project/Assets/Scenes/gfx.unity");
  system(command);
  return (0);
}
