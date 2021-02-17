# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/02/16 21:07:12 by amartin-          #+#    #+#              #
#    Updated: 2021/02/17 18:22:00 by amartin-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#PHILO_THREE
NAME		= philo_three

CC			= clang
CFLAGS		= -lpthread -Wall -Wextra -Werror #-g3 -fsanitize=address,undefined 
DEPS		= philo_three.h

SRCS		= philo_three.c state_functions.c parsing.c print.c free.c
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
