# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/04/10 17:19:11 by agrumbac          #+#    #+#              #
#    Updated: 2019/01/20 04:52:30 by agrumbac         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

############################## BIN #############################################

NAME = ft_ping

SRC = ft_ping.c in_cksum.c gen_icmp_msg.c gen_ip_header.c print_packet.c \
	errors.c socket_io.c packet_analysis.c

CC = clang

SRCDIR = srcs

OBJDIR = objs

OBJ = $(addprefix ${OBJDIR}/, $(SRC:.c=.o))

DEP = $(addprefix ${OBJDIR}/, $(SRC:.c=.d))

CFLAGS = -Wall -Wextra -Werror -fsanitize=address,undefined -g -MMD

INCLUDES = -Iincludes/

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
WB = "\033[0m""\033[34;5m"
WR = "\033[0m""\033[31;5m"
WY = "\033[0m""\033[33;5m"
X = "\033[0m"
UP = "\033[A"
CUT = "\033[K"

############################## RULES ###########################################

all: art ${NAME}

${NAME}: ${OBJ}
	@echo ${B}Compiling [${NAME}]...${X}
	@${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJ}
	@echo ${G}Success"   "[${NAME}]${X}

${OBJDIR}/%.o: ${SRCDIR}/%.c
	@echo ${Y}Compiling [$@]...${X}
	@/bin/mkdir -p ${OBJDIR}
	@${CC} ${CFLAGS} ${INCLUDES} -c -o $@ $<
	@printf ${UP}${CUT}

############################## GENERAL #########################################

clean:
	@echo ${R}Cleaning"  "[objs]...${X}
	@/bin/rm -Rf ${OBJDIR}

fclean: clean
	@echo ${R}Cleaning"  "[${NAME}]...${X}
	@/bin/rm -f ${NAME}
	@/bin/rm -Rf ${NAME}.dSYM

test:
	@${CC} -g ${INCLUDES} -fsanitize=address,undefined \
	-I. -o ${NAME} $(addprefix srcs/, ${SRC})

re: fclean all

############################## DECORATION ######################################

art:
	@echo ${BB}
	@echo "                           ."
	@echo "                          | \\/|"
	@echo "  "${WB}"(\\   _ "${X}${BB}"                 ) )|/|"
	@echo "      "${WB}"(/"${X}${BB}"            _----. /.'.'"
	@echo ".-._________..      .' "${WR}"@"${X}${BB}" "${BR}"_"${BB}"\\  .'  "${BG}"ft_ping!"${BB}
	@echo "'.._______.   '.   /    "${BR}"(_|"${BB}" .') /"
	@echo "  '._____.  /   '-/      | _.'"
	@echo "   '.______ (         ) ) \\"
	@echo "     '..____ '._       )  )"
	@echo "        .' __.--\\  , ,  // (("
	@echo "        '.'     |  \\/   (_.'(  "
	@echo "                '   \\ .'"
	@echo "                 \\   ("
	@echo "                  \\   '."
	@echo "                   \\ \\ '.)"
	@echo "                    '-'-'"
	@echo ${X}

.PHONY: all clean fclean re art

-include ${DEP}
