/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:17:07 by user42            #+#    #+#             */
/*   Updated: 2020/12/22 11:19:31 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_one.h"

int		free_philosophers(t_args **args, t_one *philo, int ret)
{
	if (philo)
		free_philosophers(args, philo->next, ret);
	if (philo && philo->thread)
	{
		pthread_detach(philo->thread);
		philo->thread = 0;
	}
	if (philo && philo->left_fork)
	{
		pthread_mutex_destroy(philo->left_fork);
		free(philo->left_fork);
	}
	if (philo && philo->index == 1)
	{
		pthread_mutex_destroy((*args)->print_lock);
		free((*args)->print_lock);
		free(*args);
	}
	if (philo)
		free(philo);
	philo = NULL;
	return (ret);
}
