NAME = rush-02 # cria variavel chamada NAME, e pode acessar usando $(NAME)

all: $(NAME)

$(NAME):
	cc -Wall -Wextra -Werror rush02.c -o $(NAME)

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
