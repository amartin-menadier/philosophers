/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:03:00 by user42            #+#    #+#             */
/*   Updated: 2020/12/10 14:27:41 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# include "../philosophers.h"
# include <pthread.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>

typedef struct	s_two{
	t_args					*args;
	sem_t					*forks;
	int						index;
	pthread_t				thread;
	size_t					time_of_death;
	int						eaten_meals;
	struct s_two			*next;
}				t_two;

void			*being_a_philosopher(void *arg);
int				free_philosophers(t_args **args, t_two *philo,
					sem_t **forks, int ret);

#endif
