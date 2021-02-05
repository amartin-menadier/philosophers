# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: user42 <user42@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/02/05 15:06:21 by user42            #+#    #+#              #
#    Updated: 2021/02/05 15:06:24 by user42           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# COLORS
_BLACK		= "\033[30m"
_RED		= "\033[31m"
_GREEN		= "\033[32m"
_YELLOW		= "\033[33m"
_BLUE		= "\033[34m"
_VIOLET		= "\033[35m"
_CYAN		= "\033[36m"
_WHITE		= "\033[37m"
_END		= "\033[0m"
_CLEAR		= "\033[2K"
_HIDE_CURS	= "\033[?25l"
_SHOW_CURS	= "\033[?25h"
_UP			= "\033[A"
_CUT		= "\033[k"

#PHILO_THREE
NAME		= philo_three

CC			= clang
CFLAGS		= -lpthread -Wall -Wextra -Werror #-g3 -fsanitize=address,undefined 
DEPS		= philo_three.h

SRCS		= philo_three.c state_functions.c parsing.c print.c
OBJS		= $(SRCS:.c=.o)

#ARGS
NUMBER_OF_PHILOSOPHERS					=     7 #philosophers
TIME_TO_DIE								= 15000 #ms to die
TIME_TO_EAT								=  200 #ms to eat
TIME_TO_SLEEP							=   5#ms to sleep
NUMBER_OF_TIME_EACH_PHILOSOPHER_MUST_EAT= 	   #meals needed


all:$(NAME)

$(NAME): $(SRCS) $(DEPS)
	$(CC) $(SRCS) -o $(NAME) $(CFLAGS) 
	
clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

launch: re
	./$(NAME) \
	$(NUMBER_OF_PHILOSOPHERS) \
	$(TIME_TO_DIE) \
	$(TIME_TO_EAT) \
	$(TIME_TO_SLEEP) \
	$(NUMBER_OF_TIME_EACH_PHILOSOPHER_MUST_EAT)

re: fclean all 
	rm -rf $(OBJS)
