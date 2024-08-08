# COLORS:
NC      =   \033[0m
LRED    =   \033[0;31m
LREDB   =   \033[1;31m
GREEN   =   \033[1;32m
LGREEN  =   \033[1;92m
PINK    =   \033[1;95m
YELLOW  =   \033[1;35m
LBLUE   =   \033[1;34m
TITLE   =   \033[1;36m

#LIBFT

LIBFT_PATH		=	libft/
LIBFT_SRCS		=	ft_atoi.c \
                	ft_atof.c \
                	ft_bzero.c \
                	ft_calloc.c \
                	ft_intarrcpy.c \
                	ft_isalnum.c \
                	ft_isalpha.c \
                	ft_isascii.c \
                	ft_isdigit.c \
                	ft_isprint.c \
                	ft_itoa.c \
                	ft_memchr.c \
                	ft_memcmp.c \
                	ft_memcpy.c \
                	ft_memmove.c \
                	ft_memset.c \
                	ft_split.c \
                	ft_strchr.c \
                	ft_strdup.c \
                	ft_strjoin.c \
                	ft_striteri.c \
                	ft_strlcat.c \
                	ft_strlcpy.c \
                	ft_strlen.c \
                	ft_strmapi.c \
                	ft_strncmp.c \
                	ft_strnstr.c \
                	ft_strrchr.c \
                	ft_substr.c \
                	ft_tolower.c \
                	ft_toupper.c \
                	ft_putchar_fd.c \
                	ft_putendl_fd.c \
                	ft_putnbr_fd.c \
                	ft_putstr_fd.c \
                	ft_utoa.c \
                	ft_strrev.c \
                	ft_strtolower.c \
                	ft_matrix_len.c \
                	ft_is_in_array.c \
                	ft_isnumber.c \
                	ft_is_int.c \
                	ft_lstnew.c \
                	ft_lstadd_front.c \
                	ft_lstsize.c \
                	ft_lstlast.c \
                	ft_lstadd_back.c \
                	ft_lstdelone.c \
                	ft_lstclear.c \
                	ft_lstiter.c \
                	ft_lstmap.c \
					ft_free_matrix.c \
					ft_free.c \
					ft_strstr.c \
					ft_substr_rev.c \
					ft_is_in_str.c

#FT_PRINTF

FT_PRINTF_PATH	=	ft_printf/
FT_PRINTF_SRCS	=	ft_printf.c \
					ft_printf_funcs.c \
					ft_print_hex.c

#GET_NEXT_LINE

GNL_PATH 		=	get_next_line/
GNL_SRCS		=	get_next_line_utils_bonus.c \
					get_next_line_bonus.c

#VARS
CC 					= 	gcc
CFLAGS				=	-Werror -Wextra -Werror
NAME				=	libft.a
INCLUDE				=	-I include/
OBJS_PATH			=	objs/
LIBFT_OBJS			=	$(addprefix $(OBJS_PATH), $(LIBFT_SRCS:.c=.o))
FT_PRINTF_OBJS		=	$(addprefix $(OBJS_PATH), $(FT_PRINTF_SRCS:.c=.o))
GNL_OBJS			=	$(addprefix $(OBJS_PATH), $(GNL_SRCS:.c=.o))


all:	 $(NAME)

$(OBJS_PATH):
	@mkdir -p $@
	@echo "$(LBLUE) \n **** [COMPILING LIBFT...] **** \n$(NC)"

$(OBJS_PATH)%.o: $(LIBFT_PATH)%.c | $(OBJS_PATH)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@echo " $<$(LGREEN) ✔ $(NC)"

$(OBJS_PATH)%.o: $(FT_PRINTF_PATH)%.c | $(OBJS_PATH)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
		@echo " $<$(LGREEN) ✔ $(NC)"

$(OBJS_PATH)%.o: $(GNL_PATH)%.c | $(OBJS_PATH)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@echo " $<$(LGREEN) ✔ $(NC)"

$(NAME): $(OBJS_PATH) $(LIBFT_OBJS) $(FT_PRINTF_OBJS) $(GNL_OBJS)
	@ar rcs $@ $(LIBFT_OBJS) $(FT_PRINTF_OBJS) $(GNL_OBJS)
	@echo "$(LGREEN) \n **** [LIBFT COMPILED] **** \n $(NC)"

clean:
	@rm -rf $(OBJS_PATH)
	@echo " $(OBJS_PATH) DELETED $(LGREEN)✔$(NC)"

fclean:	clean
	@rm -rf $(NAME)
	@echo " $(NAME) DELETED $(LGREEN)✔$(NC)"


re: 	fclean all