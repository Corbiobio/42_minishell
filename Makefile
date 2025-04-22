NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LIB = libft.a
LIB_DIR = libft/

SRC_DIR = parser/
INC_DIR = includes/
OBJ_DIR = obj/

I = -I $(INC_DIR) -I $(LIB_DIR)

HEADERS = $(INC_DIR)minishell.h $(INC_DIR)parser.h

SRC = lexer.c\
	  grammar.c\
	  io.c\
	  heredoc_bonus\
	  parser.c\

SRC_FILES = $(addprefix $(SRC_DIR), $(SRC))

OBJ_FILES = $(SRC_FILES:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

all: makelibft $(NAME)

$(OBJ_DIR):
		mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o:$(SRC_DIR)%.c $(HEADERS) | $(OBJ_DIR)
				$(CC) $(CFLAGS) $(I) -c $< -o $@

$(NAME): $(LIB_DIR)$(LIB) $(OBJ_FILES) Makefile
	$(CC) $(CFLAGS) $(OBJ_FILES) $(I) $(LIB_DIR)$(LIB) -o $(NAME)

makelibft:
		@$(MAKE) --no-print-directory -C $(LIB_DIR)

makemlx:
		@$(MAKE) --no-print-directory -C minilibx-linux

clean:
	@$(MAKE) --no-print-directory clean -C $(LIB_DIR)
	rm -rf $(OBJ_DIR)

fclean:
	@$(MAKE) --no-print-directory clean
	@$(MAKE) fclean --no-print-directory -C $(LIB_DIR)
	rm -f $(NAME) a.out

re:
	@$(MAKE) --no-print-directory fclean
	@$(MAKE) --no-print-directory all

.PHONY: all clean fclean re makelibft
