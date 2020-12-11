/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:03:00 by user42            #+#    #+#             */
/*   Updated: 2020/12/10 13:00:50 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include "../philosophers.h"
# include <pthread.h>

typedef struct	s_one{
	t_args					*args;
	int						index;
	pthread_mutex_t			*left_fork;
	pthread_mutex_t			*right_fork;
	pthread_t				thread;
	size_t					time_of_death;
	int						eaten_meals;
	struct s_one			*next;
}				t_one;

void			*being_a_philosopher(void *arg);
int				free_philosophers(t_args **args, t_one *one, int ret);

#endif
