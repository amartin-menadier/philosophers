/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:03:00 by user42            #+#    #+#             */
/*   Updated: 2021/02/11 00:57:12 by amartin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# include <pthread.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <semaphore.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <string.h>

# define FORK	" has taken a fork"
# define EAT	" is eating"
# define SLEEP	" is sleeping"
# define THINK	" is thinking"
# define DIE	" died"
# define YUMMY	" Everybody was fed! :)"
# define FULL	5

# define ARG_COUNT		1
# define ARG_FORMAT		2

typedef enum	e_state{
	DEAD,
	THINKING,
	TAKING_FORK,
	EATING,
	SLEEPING
}				t_state;

typedef struct	s_args{
	int			philo_count;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			times_must_eat;
	sem_t		*fork_pairs;
	sem_t		**lock;
	size_t		start_time;
}				t_args;

typedef struct	s_three{
	t_args		*args;
	int			index;
	t_state		state;
	pthread_t	thread;
	size_t		time_of_death;
	int			eaten_meals;
	size_t		start_time;
}				t_three;

typedef int		(*t_function)(t_three *philo, int *i, sem_t **lock);
int				parse_args(t_args *arg, int argc, char **argv);
size_t			get_time(void);
void			print_activity(size_t time, int i, char *act, sem_t **lock);
void			*being_a_philosopher(void *arg);

#endif
