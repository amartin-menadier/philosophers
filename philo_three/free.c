/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 18:13:25 by amartin-          #+#    #+#             */
/*   Updated: 2021/02/17 19:36:45 by amartin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_three.h"

int		free_philosophers(t_args * args, pid_t *pid, int ret)
{
	if (args)
	{
		if (args->lock)
		{
			sem_close(args->lock);
			sem_unlink("/lock");
		}
		if (args->fork_pairs)
		{
			sem_close(args->fork_pairs);
			sem_unlink("/forks");
		}
		free(args);
	}
	if (pid)
		free(pid);
	return (ret);
}