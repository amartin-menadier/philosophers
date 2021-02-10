/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:03:00 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 15:56:09 by amartin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <string.h>

# define FORK		" has taken a fork"
# define FORKL		" has taken L fork"
# define FORKR		" has taken R fork"
# define EAT		" is eating"
# define SLEEP		" is sleeping"
# define THINK		" is thinking"
# define DIE		" died"
# define YUMMY		" Everybody was fed! :)"
# define FULL		5

# define ARG_COUNT		1
# define ARG_FORMAT		2

typedef enum		e_state{
	DEAD,
	THINKING,
	TAKING_FORK,
	EATING,
	SLEEPING
}					t_state;

typedef struct		s_args{
	int					number_of_philosophers;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					times_must_eat;
	size_t				start_time;
	pthread_mutex_t		*print_lock;
}					t_args;

typedef struct		s_one{
	t_args				*args;
	int					index;
	t_state				state;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_t			thread;
	size_t				time_activity_end;
	size_t				time_of_death;
	int					eaten_meals;
	struct s_one		*next;
}					t_one;

typedef int			(*t_function)(t_one *philo, int *i, pthread_mutex_t *lock);
int					parse_args(t_args *arg, int argc, char **argv);
size_t				get_time(void);
void				print_activity(size_t time, int index, char *activity,
						pthread_mutex_t *lock);
void				*being_a_philosopher(void *arg);
int					free_philosophers(t_args **args, t_one *one, int ret);

#endif
