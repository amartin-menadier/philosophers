/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:17:07 by user42            #+#    #+#             */
/*   Updated: 2020/12/07 19:24:29 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_two.h"

int		free_philosophers(t_args **args, t_two **philo, sem_t **forks, int ret)
{
	if ((*philo)->next)
		free_philosophers(args, &((*philo)->next), forks, ret);
	if ((*philo)->thread)
	{
		sem_post(*forks);
		pthread_join((*philo)->thread, NULL);
		(*philo)->thread = 0;
	}
	if ((*philo)->index == 1 && args && *args && forks && *forks)
	{
		sem_close(*forks);
		sem_unlink("/forks");
		(*forks) = NULL;
		free(*args);
		(*args) = NULL;
		write(1, "\nEND OF PHILO_TWO SIMULATION\n", 30);
	}
	free(*philo);
	*philo = NULL;
	return (ret);
}
