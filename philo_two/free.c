/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:17:07 by user42            #+#    #+#             */
/*   Updated: 2020/12/10 14:39:45 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_two.h"

int		free_philosophers(t_args **args, t_two *philo, sem_t **forks, int ret)
{
	if (philo && philo->next)
		free_philosophers(args, philo->next, forks, ret);
	if (philo && philo->thread)
	{
		sem_post(*forks);
		pthread_join(philo->thread, NULL);
		philo->thread = 0;
	}
	if ((!philo || (philo && philo->index == 1)) && args && *args)
	{
		if (forks && *forks)
			sem_close(*forks);
		sem_unlink("/forks");
		(*forks) = NULL;
		free(*args);
		(*args) = NULL;
	}
	free(philo);
	philo = NULL;
	return (ret);
}
