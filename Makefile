NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LIB = libft.a
LIB_DIR = libft/

PARS_DIR = parser/
EXEC_DIR = execution/
INC_DIR = includes/
OBJ_DIR = obj/

I = -I $(INC_DIR) -I $(LIB_DIR)

HEADERS = $(INC_DIR)minishell.h $(INC_DIR)parser.h

PARS =\
	lexer.c\
	grammar.c\
	io.c\
	parser.c\

PARS_FILES = $(addprefix $(PARS_DIR), $(PARS))

EXEC =\
	execution.c\

EXEC_FILES = $(addprefix $(EXEC_DIR), $(EXEC))

SRC_FILES = $(PARS_FILES) $(EXEC_FILES)

OBJ_FILES = $(SRC_FILES:.c=.o)
OBJ_FILES := $(addprefix $(OBJ_DIR), $(OBJ_FILES))

all: makelibft $(NAME)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o:$(SRC_DIR)%.c $(HEADERS) | $(OBJ_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(I) -c $< -o $@

$(NAME): $(LIB_DIR)$(LIB) $(OBJ_FILES) Makefile
	$(CC) $(CFLAGS) $(OBJ_FILES) $(I) $(LIB_DIR)$(LIB) -o $(NAME)

makelibft:
	@$(MAKE) --no-print-directory -C $(LIB_DIR)

clean:
	@$(MAKE) --no-print-directory clean -C $(LIB_DIR)
	rm -rf $(OBJ_DIR)

fclean:
	@$(MAKE) --no-print-directory clean
	@$(MAKE) fclean --no-print-directory -C $(LIB_DIR)
	rm -f $(NAME)

re:
	@$(MAKE) --no-print-directory fclean
	@$(MAKE) --no-print-directory all

.PHONY: all clean fclean re makelibft
