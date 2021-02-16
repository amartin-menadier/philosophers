/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:17:07 by user42            #+#    #+#             */
/*   Updated: 2021/02/16 20:52:10 by amartin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_one.h"

void	destroy_forks(t_one *philo)
{
	while (philo)
	{
		pthread_mutex_destroy(philo->left_fork);
		free(philo->left_fork);
		philo = philo->next;
	}
}

void	unlock_forks(t_one *philo)
{
	while (philo)
	{
		pthread_mutex_unlock(philo->left_fork);
		philo = philo->next;
	}
}

void	free_args(t_args **args)
{
	if ((*args)->print_lock)
	{
		pthread_mutex_destroy((*args)->print_lock);
		free((*args)->print_lock);
	}
	free(*args);
	*args = NULL;
}

int		free_philosophers(t_args **args, t_one *philo, int ret)
{
	t_one	*tmp;

	tmp = philo;
	unlock_forks(philo);
	usleep((*args)->time_to_die * 1000);
	philo = tmp;
	destroy_forks(philo);
	philo = tmp;
	while (philo && philo->thread)
	{
		pthread_join(philo->thread, NULL);
		pthread_detach(philo->thread);
		philo->thread = 0;
		philo = philo->next;
	}
	if (args && *args)
		free_args(args);
	philo = tmp;
	while (philo)
	{
		tmp = philo->next;
		free(philo);
		philo = tmp;
	}
	return (ret);
}
