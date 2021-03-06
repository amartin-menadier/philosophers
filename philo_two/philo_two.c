/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:04:16 by user42            #+#    #+#             */
/*   Updated: 2021/02/16 22:06:39 by amartin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_two.h"

static int	free_philosophers(t_args *args, t_two **philo, int ret)
{
	if (*philo && (*philo)->next)
		free_philosophers(args, &(*philo)->next, ret);
	pthread_join((*philo)->thread, NULL);
	(*philo)->thread = 0;
	if (*philo && (*philo)->index == 1 && args)
	{
		sem_close(args->lock);
		sem_unlink("/lock");
		sem_close(args->fork_pairs);
		sem_unlink("/forks");
		free(args);
	}
	if (*philo)
		free(*philo);
	*philo = NULL;
	return (ret);
}

static int	start_simulation(t_two *philo, t_args *args)
{
	if (!philo)
		return (EXIT_SUCCESS);
	pthread_create(&philo->thread, NULL, being_a_philosopher, philo);
	return (start_simulation(philo->next, args));
}

static void	wait_till_the_end(t_two *philo, t_args *args, sem_t *lock)
{
	t_two	*first;
	int		full_philosophers;

	first = philo;
	full_philosophers = 0;
	while (full_philosophers < args->philo_count)
	{
		if (philo->state && philo->eaten_meals == args->times_must_eat)
		{
			philo->state = DEAD;
			full_philosophers++;
		}
		if (get_time() > philo->time_of_death
			&& philo->eaten_meals != args->times_must_eat)
			break ;
		if (philo->next)
			philo = philo->next;
		else
			philo = first;
	}
	args->times_must_eat = -2;
	if (full_philosophers != args->philo_count)
		print_activity(get_time() - args->start_time, philo->index, DIE, lock);
	usleep(200);
	return ;
}

static int	recruit_philosophers(t_args *args, t_two **philo, int index)
{
	if (index != 1 && !((*philo) = (t_two *)malloc(sizeof(t_two))))
		return (EXIT_FAILURE);
	(*philo)->args = args;
	(*philo)->index = index;
	(*philo)->state = THINKING;
	(*philo)->time_of_death = 0;
	(*philo)->eaten_meals = 0;
	if (index == args->philo_count)
		(*philo)->next = NULL;
	else
		return (recruit_philosophers(args, &(*philo)->next, index + 1));
	return (EXIT_SUCCESS);
}

int			main(int argc, char **argv)
{
	t_args	*arg;
	t_two	*philo;

	arg = NULL;
	philo = NULL;
	if (!(arg = malloc(sizeof(t_args))))
		return (EXIT_FAILURE);
	if (parse_args(arg, argc, argv)
		|| !(philo = malloc(sizeof(t_two))))
	{
		free(arg);
		return (EXIT_FAILURE);
	}
	philo->next = NULL;
	philo->index = 1;
	if (arg->fork_pairs == SEM_FAILED || arg->lock == SEM_FAILED
		|| recruit_philosophers(arg, &philo, 1)
		|| start_simulation(philo, arg))
		return (free_philosophers(arg, &philo, EXIT_FAILURE));
	while (!arg->start_time)
		usleep(1000);
	wait_till_the_end(philo, arg, arg->lock);
	return (free_philosophers(arg, &philo, EXIT_SUCCESS));
}
