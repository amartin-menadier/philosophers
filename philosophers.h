/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:03:00 by user42            #+#    #+#             */
/*   Updated: 2020/12/04 21:56:47 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

# define FORK	"has taken a fork"
# define EAT	"is eating"
# define SLEEP	"is sleeping"
# define THINK	"is thinking"
# define DIE	"died"

/*
typedef enum	e_activity{
	DEAD,
	TAKING_A_FORK,
	EATING,
	SLEEPING,
	THINKING
}				e_activity;
*/

typedef struct	s_args{
	size_t		number_of_philosophers;
	size_t		time_to_die;
	size_t		time_to_eat;
	size_t		time_to_sleep;
	size_t		times_philo_must_eat;
	size_t		start_time;
}				t_args;

int				parse_args(t_args *arg, int argc, char **argv);
char			*basic_itoa(size_t i);
int				basic_strlen(char *str);
size_t			get_time(void);

#endif
