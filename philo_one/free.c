/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:17:07 by user42            #+#    #+#             */
/*   Updated: 2020/12/07 18:18:01 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_one.h"

int		free_philosophers(t_args **args, t_one **philo, int ret)
{
	if ((*philo)->next)
		free_philosophers(args, &((*philo)->next), ret);
	if ((*philo)->thread)
	{
		pthread_join((*philo)->thread, NULL);
		(*philo)->thread = 0;
	}
	if ((*philo)->left_fork)
	{
		pthread_mutex_destroy((*philo)->left_fork);
		free((*philo)->left_fork);
	}
	if ((*philo)->index == 1 && args && *args)
	{
		if ((*args)->number_of_philosophers == 1)
		{
			pthread_mutex_destroy((*philo)->right_fork);
			free((*philo)->right_fork);
		}
		free(*args);
		write(1, "\nEND OF PHILO_ONE SIMULATION\n", 30);
	}
	free(*philo);
	*philo = NULL;
	return (ret);
}
