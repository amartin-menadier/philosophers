/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:17:07 by user42            #+#    #+#             */
/*   Updated: 2020/12/10 14:50:51 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_three.h"

int		free_philosophers(t_args **args, pid_t **pids, int ret)
{
	int	i;

	i = 0;
	while ((*pids) && i < (*args)->number_of_philosophers)
	{
		kill((*pids)[i], SIGKILL);
		i++;
	}
	sem_close((*args)->forks);
	sem_unlink("/forks");
	sem_close((*args)->lock);
	sem_unlink("/lock");
	free(*args);
	(*args) = NULL;
	free(*pids);
	*pids = NULL;
	return (ret);
}
