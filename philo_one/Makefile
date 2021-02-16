# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/02/16 21:07:32 by amartin-          #+#    #+#              #
#    Updated: 2021/02/16 21:07:38 by amartin-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#PHILO_ONE
NAME		= philo_one

CC			= clang
CFLAGS		= -lpthread -Wall -Wextra -Werror #-g3 -fsanitize=address,undefined 
DEPS		= philo_one.h

SRCS		= philo_one.c state_functions.c free.c parsing.c print.c
OBJS		= $(SRCS:.c=.o)

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
