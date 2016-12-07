NAME = client

HEADER = includes/client.hpp

SRC = src/client_main.cpp

OBJ = $(SRC:.cpp=.o)

CC = g++ -g -Wall -Werror -Wextra

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
