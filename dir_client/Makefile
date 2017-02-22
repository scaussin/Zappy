NAME =		client

CC =		g++ $(FLAGS) $(INCLUDES)

INCLUDES =	-I./includes

SRC = 		src/main.cpp \
			src/ClientInputHandler.cpp \
			src/ClientCommunication.cpp \
			src/ClientPlayer.cpp \
			src/CustomException.cpp \
			src/ClientBuffer.cpp \
			src/ClientIa.cpp \
			src/ClientIaCheck.cpp \
			src/ClientIaFindItem.cpp \
			src/ClientIaLevelUp.cpp \
			src/ClientIaNewClient.cpp \
			src/ClientIaItemsPose.cpp \
			src/ClientIaCheckBeforeBroadcast.cpp \
			src/CallbackCommand.cpp \
			src/ClientController.cpp

OBJ =		$(SRC:%.cpp=%.o)

FLAGS =		-Wall -Wextra -Werror -g -std=c++11

all :		$(NAME)

$(NAME):	$(OBJ)
			$(CC) $(OBJ) -o $(NAME)

clean :
			rm -rf $(OBJ)

fclean : 	clean
			rm -rf $(NAME)

re : 		fclean all

%.o:		%.cpp
				$(CC) -c $< -o $@

.PHONY : all clean fclean re
