/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:03:00 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 16:24:26 by amartin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# include <pthread.h>
# include <fcntl.h>
# include <sys/stat.h>
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

typedef enum		e_state{
	DEAD,
	THINKING,
	TAKING_FORK,
	EATING,
	SLEEPING
}					t_state;

typedef struct	s_args{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_must_eat;
	sem_t			*forks;
	sem_t			*lock;
	size_t			start_time;
}				t_args;

typedef struct	s_two{
	t_args			*args;
	int				index;
	t_state			state;
	pthread_t		thread;
	size_t			time_of_death;
	size_t			time_activity_end;
	int				eaten_meals;
	struct s_two	*next;
}				t_two;

typedef int		(*t_function)(t_two *philo, int *i, sem_t *lock);
int				parse_args(t_args *arg, int argc, char **argv);
size_t			get_time(void);
void			print_activity(size_t time, int index, char *act, sem_t *lock);
void			*being_a_philosopher(void *arg);

#endif
