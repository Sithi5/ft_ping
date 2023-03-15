NAME1				=	ft_ping

SANITIZE			=	no
WEVERYTHING			=	no

ifeq ($(SANITIZE),yes)
	CC				=	gcc -Wall -Wextra -Werror -fsanitize=address \
						-Wall -Wextra -Werror
else ifeq ($(WEVERYTHING),yes)
	CC				=	gcc -Wall -Wextra -Weverything
else
	CC				=	gcc -Wall -Wextra -Werror
endif

SRCS_NAME			=	ft_ping.c \
						signal_handlers.c

INCLUDES_NAME		=	ft_ping.h

SRCS_PATH			=	./srcs/
INCLUDES_PATH		=	./includes/

OBJS_PATH 			=	./objs/

INCLUDES  			=	$(addprefix $(INCLUDES_PATH), $(INCLUDES_NAME))

SRCS				=	$(addprefix $(SRCS_PATH), $(SRCS_NAME))

OBJS			=	$(patsubst $(SRCS_PATH)%.c, $(OBJS_PATH)%.o, $(SRCS))

.PHONY				:	all lib clean fclean re

_UNDER=$'\x1b[4m$'
_GREEN=$'\x1b[32m$'
_YELLOW=$'\x1b[33m$'
_WHITE=$'\x1b[37m$'
_END=$'\x1b[0m$'
_BOLD=$'\x1b[1m$'

all: $(NAME1)

$(NAME1): $(OBJS) $(INCLUDES)
	@echo "$(_WHITE)====================================================$(_END)"
	@echo "$(_YELLOW)		COMPILING FT_PING$(_END)"
	@echo "$(_WHITE)====================================================$(_END)"
	@$(CC) -o $(NAME1) $(OBJS)
	@echo "\n$(_WHITE)$(_BOLD)$@\t$(_END)$(_GREEN)[OK]\n$(_END)"
ifeq ($(SANITIZE),yes)
	@echo "Génération en mode sanitize"
else ifeq ($(WEVERYTHING),yes)
	@echo "Génération en mode weverything"
else
	@echo "Génération en mode release"
endif

clean:
	@echo "$(_RED)"
	rm -rf $(OBJS)
	rm -rf ./objs

fclean:	clean
	@echo "$(_YELLOW)"
	rm -f $(NAME1)


re:	fclean all

$(OBJS_PATH)%.o: $(SRCS_PATH)%.c $(INCLUDES)
	@mkdir -p ./objs/
	@$(CC) -I $(LIBFTINCLUDES) \
		-I $(INCLUDES_PATH) -c $< -o $@
	@echo "$(_END)$(_GREEN)[OK]\t$(_UNDER)$(_YELLOW)	\
		COMPILE :$(_END)$(_BOLD)$(_WHITE)\t$<"