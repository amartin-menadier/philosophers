/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:03:00 by user42            #+#    #+#             */
/*   Updated: 2020/12/10 14:45:33 by user42           ###   ########.fr       */
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

typedef struct	s_args{
	int			number_of_philosophers;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			times_must_eat;
	size_t		start_time;
	sem_t		*lock;
	sem_t		*forks;
}				t_args;

typedef struct	s_three{
	t_args		*args;
	int			index;
	pthread_t	thread;
	size_t		time_of_death;
	int			eaten_meals;
}				t_three;

int				parse_args(t_args *arg, int argc, char **argv);
size_t			get_time(void);
void			print_activity(size_t time, int index,
					char *activity, sem_t *lock);
void			*being_a_philosopher(void *arg);
int				free_philosophers(t_args **args, pid_t **pids, int ret);

#endif
