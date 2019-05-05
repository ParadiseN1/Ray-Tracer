NAME	= RTv1

#files
SRC		= main.c \
			secondary.c \
			sdl_funcs.c \
			rotate.c \
			get_normal.c \
			hit_or_miss.c \
			render.c \
			actions.c \
			vector_operators_1.c \
			vector_operators_2.c \
			anaglyph.c \
			parser/parser.c \
			parser/add_obj_funcs.c \
			parser/get_light_sources.c \
			parser/cleaners.c \
			parser/gimy_gimy.c \
			parser/pokemons.c 
	

OBJ		= $(addprefix $(OBJDIR), $(SRC:.c=.o))

FL = -F Frameworks/ -rpath Frameworks/ -framework SDL2 -F Frameworks/ -rpath Frameworks/ -framework SDL2_image  -framework SDL2_ttf
SDLINC = -F Frameworks/
#compiler
CC		= gcc
FLAGS	= -g 

#mlx library
UNAME_S = $(shell uname -s)

#ft library
FT		= ./libft/
FT_LIB	= $(addprefix $(FT), libft.a)
FT_INC	= -I ./libft/
FT_LNK	= -L ./libft/ -lft

#directories
SRCDIR	= ./src/
OBJDIR	= ./obj/

all: obj $(FT_LIB) $(NAME) 

obj:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/scenes
	@mkdir -p $(OBJDIR)/parser

$(OBJDIR)%.o:$(SRCDIR)%.c
	@$(CC) $(FLAGS) $(FT_INC) $(SDLINC)-I $(INCDIR) -o $@ -c $<
	@echo "(+) $@"

$(FT_LIB):
	@make -C $(FT)

$(MLX_LIB):
	@make -C $(MLX)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) $(FT_LNK) $(FL) -lm -o $(NAME)
	@echo "$(GRN)$(NAME) is compiled!$(EOC)"

leak:
	@valgrind --leak-check=full  ./rtv1 scene.rtv1

clean:
	@rm -rf $(OBJDIR)
	@make -C $(FT) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(FT) fclean

re: fclean all

RED = \033[1;31m
GRN = \033[1;32m
YLW = \033[1;33m
INV = \033[7m
EOC = \033[0m
