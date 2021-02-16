# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/02/16 21:07:00 by amartin-          #+#    #+#              #
#    Updated: 2021/02/16 21:07:51 by amartin-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#PHILO_TWO
NAME		= philo_two

CC			= clang
CFLAGS		= -lpthread -Wall -Wextra -Werror #-g3 -fsanitize=address,undefined 
DEPS		= philo_two.h

SRCS		= philo_two.c state_functions.c parsing.c print.c 
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
