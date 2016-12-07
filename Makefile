# *****************************************************	#
#														#
#	Global Makefile for the three binaries.				#
#														#
# *****************************************************	#

NAME_SERVEUR = serveur
NAME_CLIENT = client
NAME_GFX = gfx

DIR_SERVEUR = ./dir_serveur/
DIR_CLIENT = ./dir_client/
DIR_GFX = ./dir_gfx/


all : $(NAME_SERVEUR) $(NAME_CLIENT) $(NAME_GFX)

$(NAME_SERVEUR) :
	make -C $(DIR_SERVEUR)

$(NAME_CLIENT) :
	make -C $(DIR_CLIENT)

$(NAME_GFX) :
	make -C $(DIR_GFX)

clean :
	cd $(DIR_SERVEUR) ; make clean
	cd $(DIR_CLIENT) ; make clean
	cd $(DIR_GFX) ; make clean

fclean : clean
	cd $(DIR_SERVEUR) ; make fclean
	cd $(DIR_CLIENT) ; make fclean
	cd $(DIR_GFX) ; make fclean

re : fclean all

.PHONY : all clean fclean re
