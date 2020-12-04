/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:03:00 by user42            #+#    #+#             */
/*   Updated: 2020/12/04 23:17:15 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

//# include <time.h>
//# include <sys/time.h>
//# include <stdlib.h>
# include "../philosophers.h"
# include <pthread.h>

typedef struct	s_philo{
	t_args					*args;
	int						index;
//	char					padding0[4];
	pthread_mutex_t			*left_fork;
	pthread_mutex_t			*right_fork;
	pthread_t				thread;
	size_t					time_of_death;
	struct s_philo			*next;
}				t_philo;

#endif
