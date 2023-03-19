SHELL				=	/bin/sh

################################################################################
#                                BINARIES NAME                                 #
################################################################################

NAME				=	ft_ping

ART_NAME			=	bird

################################################################################
#                                COMPILATION MODE                              #
################################################################################

WALL				:=	no
WEXTRA				:=	no
WSHADOW				:=	no
WERROR				:=	no
FSANITIZE			:=	no
PREPROCESSONLY		:=	no
DEBUG				:=	no
O2					:=	no

CC					:= gcc
GEN					:= Generation in mode

ifeq ($(WALL), yes)
	CC				+=	-Wall
	GEN				+=	 all
endif

ifeq ($(WEXTRA), yes)
	CC				+=	-Wextra
	GEN				+=	extra
endif

ifeq ($(WEXTRA), yes)
	CC				+=	-E
	GEN				+=	preprocessonly
endif

ifeq ($(WSHADOW), yes)
	CC				+=	-Wshadow
	GEN				+=	shadow
endif

ifeq ($(WERROR), yes)
	CC				+=	-Werror
	GEN				+=	error
endif

ifeq ($(FSANITIZE), yes)
	CC				+=	-fsanitize=address
	GEN				+=	sanitize
endif

ifeq ($(DEBUG), yes)
	CC				+=	-g
	CC				+= -D DEBUG
	GEN				+=	debug
endif

ifeq ($(O2),yes)
	CC				+=	-O2
	GEN				+=	O2
endif

ifeq ($(GEN), Generation in mode)
	GEN				+=	no flags
endif



################################################################################
#                                     NAME                                     #
################################################################################

SRC_NAME			:=	ft_ping.c							\
						utils.c 							\
						prints.c 							\
						struct.c 							\
						args.c 								\
						send_packages.c 					\
						receive_packages.c 					\
						errors.c 							\
						network.c


INCLUDE_NAME		:=	ft_ping.h

TESTS_SRC_NAME		:= 	./tests/test*.sh					\

################################################################################
#                                     PATH                                     #
################################################################################

SRC_PATH			:=	./src/

OBJ_PATH 			:=	./obj/

INCLUDE_PATH		:=	./include/

################################################################################
#                                 NAME + PATH                                  #
################################################################################

SRC					:=	$(addprefix $(SRC_PATH), $(SRC_NAME))

OBJ					:=	$(patsubst $(SRC_PATH)%.c, $(OBJ_PATH)%.o,	$(SRC))

INCLUDE				:=	$(addprefix $(INCLUDE_PATH), $(INCLUDE_NAME))


################################################################################
#                                     RULES                                    #
################################################################################

all: $(ART_NAME) $(NAME)

$(NAME): $(OBJ)
	@echo "\n$(NAME) : $(GEN)"
	@echo "\n$(_CYAN)====================================================$(_END)"
	@echo "$(_YELLOW)		COMPILING $(NAME)$(_END)"
	@echo "$(_CYAN)====================================================$(_END)"
	@$(CC) -o $(NAME) $(OBJ)
	@echo "\n$(_WHITE)$(_BOLD)$@\t$(_END)$(_GREEN)[OK]\n$(_END)"
	@echo "\n"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(INCLUDE)
	@mkdir -p $(OBJ_PATH)
	@$(CC) -I $(INCLUDE_PATH) -c $< -o $@
	@echo "$(_END)$(_GREEN)[OK]\t$(_UNDER)$(_YELLOW)\t"	\
		"COMPILE :$(_END)$(_BOLD)$(_WHITE)\t$<"

tests: all
	@echo "\n$(_CYAN)====================================================$(_END)"
	@echo "$(_YELLOW)		LAUNCHING TESTS$(_END)"
	@echo "$(_CYAN)====================================================$(_END)"
	@for f in $(TESTS_SRC_NAME);  do sh $${f}; done;

clean:
	@rm -rf $(OBJ_PATH) 2> /dev/null || true
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(OBJ_PATH)"$(_END)"
	@rm -f $(ART_NAME)
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(ART_NAME)"$(_END)"


fclean: clean
	@rm -f $(NAME)
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(NAME)
	@echo "$(_END)"

re: fclean all

help:
	@echo "$(_YELLOW)Makefile for generating binary infectors."
	@echo "$(_YELLOW)Usage:                                                                    "
	@echo "$(_YELLOW)   make                                runs all                           "
	@echo "$(_YELLOW)   make all                            generates all binaries             "
	@echo "$(_YELLOW)   make clean                          remove the generated files         "
	@echo "$(_YELLOW)   make fclean                  		clean and remove binaries files    "
	@echo "$(_YELLOW)   make tests                          launch tests scripts               "
	@echo "$(_YELLOW)   make help                           prints this message                $(_END)"


$(ART_NAME):
	@touch $(ART_NAME)
	@echo "$(_CYAN)" > $(ART_NAME)
	@echo "       '-------------------.. " >> $(ART_NAME)
	@echo "         '\\_________         \`-.-----. " >> $(ART_NAME)
	@echo "             '\\_______          \`\\    \`.                   $(_GREEN)  __   ____ $(_CYAN)" >> $(ART_NAME)
	@echo "                  \\____           \`\\   :                    $(_GREEN)/_ | | .-.\\ $(_CYAN)" >> $(ART_NAME)
	@echo "    '._____         (____           \`\\.\`.             .--._ $(_GREEN)  \\\\' / $(_CYAN)" >> $(ART_NAME)
	@echo "     \\____  \"'..____.(_______          \`~-.________.-'  $(_YELLOW)@$(_CYAN). \\___${_GREEN}\\.'${_CYAN}___ " >> $(ART_NAME)
	@echo "     )___     ___                                         /___________\\ " >> $(ART_NAME)
	@echo "      )___..''   '--.                    _.----------.____\`----$(_GREEN))'($(_CYAN)---' " >> $(ART_NAME)
	@echo "                   _:-'   ,          _.-'                $(_GREEN)     /( \\\\ $(_CYAN)" >> $(ART_NAME)
	@echo "               .-~~ __..-'~~~~~~~~~~'                    $(_GREEN)   .'    '\\ $(_CYAN)" >> $(ART_NAME)
	@echo "               \\ \\~~ \\ \\                             $(_GREEN)      / '    ) \\ $(_CYAN)" >> $(ART_NAME)
	@echo "                \\ \\   \\ \\_.-'                        $(_GREEN)     |      )   ' $(_CYAN)" >> $(ART_NAME)
	@echo "                 \\ \\_ _;-,,'                           $(_GREEN)   ( (      ) ) $(_CYAN)" >> $(ART_NAME)
	@echo "                 ;-,,'                                   $(_GREEN)  \"--------\" $(_CYAN)" >> $(ART_NAME)
	@echo "" >> $(ART_NAME)
	@echo "$(_END)" >> $(ART_NAME)
	@cat $(ART_NAME)

.PHONY: all clean fclean re check help tests clean_payload

################################################################################
#                                    FORMAT                                    #
################################################################################

# Text format
_END		=	\033[0m
_GRAS		=	\033[1m
_SOUL		=	\033[4m
_CLIG		=	\033[5m
_SURL		=	\033[7m

# Colors format
_BLACK		=	\033[30m
_RED		=	\033[31m
_GREEN		=	\033[32m
_YELLOW		=	\033[33m
_BLUE		=	\033[34m
_PURPLE		=	\033[35m
_CYAN		=	\033[36m
_GREY		=	\033[37m

# Background
_IBLACK		=	\033[40m
_IRED		=	\033[41m
_IGREEN		=	\033[42m
_IYELLOW	=	\033[43m
_IBLUE		=	\033[44m
_IPURPLE	=	\033[45m
_ICYAN		=	\033[46m
_IGREY		=	\033[47m