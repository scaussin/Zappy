NAME = serveur

HEADER = includes/serveur.h

SRC = src/serveur_main.c \
src/connection.c \
src/communication.c \
src/communication_1.c \
src/communication_2.c \
src/communication_gfx.c \
src/communication_gfx_1.c \
src/tools.c \
src/tools_1.c \
src/tools_2.c \
src/cmd_clients_init.c \
src/cmd_clients_manager.c \
src/cmd_clients_manager_1.c \
src/input_handler.c \
src/input_handler_1.c \
src/client_hdl.c \
src/client_authentification.c \
src/client_authentification_1.c \
src/team_hdl.c \
src/serveur_data.c \
src/terrain_generation.c \
src/terrain_generation_1.c \
src/serveur_loop.c \
src/game_events.c \
src/game_events_1.c \
src/game_player_events.c \
src/game_events_helper.c \
src/egg_handling.c \
src/player_handler.c \
src/clock.c \
src/clock_1.c \
src/cmds_functions/cmd_avance.c \
src/cmds_functions/cmd_droite.c \
src/cmds_functions/cmd_gauche.c \
src/cmds_functions/cmd_voir.c \
src/cmds_functions/cmd_voir_1.c \
src/cmds_functions/cmd_inventaire.c \
src/cmds_functions/cmd_prend.c \
src/cmds_functions/cmd_pose.c \
src/cmds_functions/cmd_expulse.c \
src/cmds_functions/cmd_expulse_1.c \
src/cmds_functions/cmd_broadcast.c \
src/cmds_functions/cmd_broadcast_1.c \
src/cmds_functions/cmd_broadcast_2.c \
src/cmds_functions/cmd_broadcast_3.c \
src/cmds_functions/cmd_incantation.c \
src/cmds_functions/cmd_incantation_1.c \
src/cmds_functions/cmd_incantation_2.c \
src/cmds_functions/cmd_incantation_3.c \
src/cmds_functions/cmd_fork.c \
src/cmds_functions/cmd_connect_nbr.c \
src/gfx_cmds_functions/gfx_cmd_msz.c \
src/gfx_cmds_functions/gfx_cmd_bct.c \
src/gfx_cmds_functions/gfx_cmd_mct.c \
src/gfx_cmds_functions/gfx_cmd_tna.c \
src/gfx_cmds_functions/gfx_cmd_ppo.c \
src/gfx_cmds_functions/gfx_cmd_plv.c \
src/gfx_cmds_functions/gfx_cmd_pin.c \
src/gfx_cmds_functions/gfx_cmd_sgt.c \
src/gfx_cmds_functions/gfx_cmd_sst.c

OBJ = $(SRC:.c=.o)

CC = gcc -g -Wall -Werror -Wextra

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
