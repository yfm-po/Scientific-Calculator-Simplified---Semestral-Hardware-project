NAME		= Calculate
PATH_SRC	= ./
HEADER		= ./
OPTION		= -I $(HEADER)
SRC			= Calculate.c removeSpaces.c cursorMover.c writeNumber.c
FLAG		= -Wall -Wextra -Werror

all: $(NAME)

$(NAME):
		gcc $(FLAG) $(OPTION) -o $(NAME) $(addprefix $(PATH_SRC), $(SRC))
clean:

fclean:	clean
		/bin/rm -f $(NAME)

re:		fclean all

.PHONY: all clean fclean re
