# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/04/10 17:19:11 by agrumbac          #+#    #+#              #
#    Updated: 2018/11/04 19:26:20 by agrumbac         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

############################## BIN #############################################

NAME = ft_ping

SRC = ft_ping.c

CC = clang

SRCDIR = srcs

OBJDIR = objs

OBJ = $(addprefix ${OBJDIR}/, $(SRC:.c=.o))

CFLAGS = -Wall -Wextra -Werror -fsanitize=address,undefined -g

LIB = -Llibft/ -lft

INCLUDES = -Ilibft/includes/ -Iincludes/

DEP = includes/ft_ping.h libft/libft.a

############################## COLORS ##########################################

BY = "\033[33;1m"
BR = "\033[31;1m"
BG = "\033[32;1m"
BB = "\033[34;1m"
BM = "\033[35;1m"
BC = "\033[36;1m"
Y = "\033[33m"
R = "\033[31m"
G = "\033[32m"
B = "\033[34m"
M = "\033[35m"
C = "\033[36m"
WT = "\033[37;1m"
W = "\033[0m""\033[32;1m"
WR = "\033[0m""\033[31;5m"
WY = "\033[0m""\033[33;5m"
X = "\033[0m"
UP = "\033[A"
CUT = "\033[K"

############################## RULES ###########################################

all: art ${NAME}

libft/%:
	@[[ -d libft ]] || (echo ${M}Cloning"   "[libft]...${X} && git clone https://github.com/grumbach/libft &>/dev/null)
	@make -C libft

${NAME}: ${OBJ}
	@echo ${B}Compiling [${NAME}]...${X}
	@${CC} ${CFLAGS} ${INCLUDES} ${LIB} -o $@ ${OBJ}
	@echo ${G}Success"   "[${NAME}]${X}

${OBJDIR}/%.o: ${SRCDIR}/%.c ${DEP}
	@echo ${Y}Compiling [$@]...${X}
	@/bin/mkdir -p ${OBJDIR}
	@${CC} ${CFLAGS} ${INCLUDES} -c -o $@ $<
	@printf ${UP}${CUT}

############################## GENERAL #########################################

clean:
	@echo ${R}Cleaning"  "[libft objs]...${X}
	@make -C libft/ clean
	@echo ${R}Cleaning"  "[objs]...${X}
	@/bin/rm -Rf ${OBJDIR}

fclean: clean
	@make -C libft/ fclean
	@echo ${R}Cleaning"  "[${NAME}]...${X}
	@/bin/rm -f ${NAME}
	@/bin/rm -Rf ${NAME}.dSYM

test: libft/libft.a
	@${CC} -g ${INCLUDES} -fsanitize=address,undefined ${LIB} \
	-I. -o ${NAME} $(addprefix srcs/, ${SRC})

re: fclean all

############################## DECORATION ######################################

art:
	@echo ${WY}
	@echo "__INSERT_ASCII_ART__"
	@echo ${X}

.PHONY: all clean fclean re art
