/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:04:16 by user42            #+#    #+#             */
/*   Updated: 2020/12/11 14:18:38 by user42           ###   ########.fr       */
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

int		start_simulation(t_two *philo, t_args *args)
{
	if (!philo)
		return (EXIT_SUCCESS);
	if (philo->index == 1)
		args->start_time = get_time();
	philo->time_of_death = args->start_time + args->time_to_die;
	pthread_create(&philo->thread, NULL, being_a_philosopher, philo);
	return (start_simulation(philo->next, args));
}

void	wait_till_the_end(t_two *philo, t_args *args, size_t start_time)
{
	t_two	*first;
	int		full_philosophers;

	first = philo;
	full_philosophers = 0;
	while (get_time() < philo->time_of_death
		&& full_philosophers < args->number_of_philosophers)
	{
		if (philo->eaten_meals == args->times_philo_must_eat)
		{
			full_philosophers++;
			philo->eaten_meals++;
		}
		if (philo->next && full_philosophers < args->number_of_philosophers)
			philo = philo->next;
		else if (full_philosophers < args->number_of_philosophers)
			philo = first;
	}
	args->times_philo_must_eat = -2;
	if (full_philosophers != args->number_of_philosophers)
		print_activity(get_time() - start_time, philo->index, DIE);
	return ;
}

int		recruit_philosophers(t_args *args, t_two **philo,
			sem_t *forks, int index)
{
	if (index != 1 && !((*philo) = (t_two *)malloc(sizeof(t_two))))
		return (EXIT_FAILURE);
	(*philo)->args = args;
	(*philo)->forks = forks;
	(*philo)->index = index;
	(*philo)->thread = -1;
	(*philo)->time_of_death = 0;
	(*philo)->eaten_meals = 0;
	if (index == args->number_of_philosophers)
		(*philo)->next = NULL;
	else
		return (recruit_philosophers(args, &(*philo)->next, forks, index + 1));
	return (EXIT_SUCCESS);
}

int		main(int argc, char **argv)
{
	t_args			*args;
	t_two			*philo;
	sem_t			*forks;

	args = NULL;
	philo = NULL;
	forks = NULL;
	if (!(args = malloc(sizeof(t_args)))
		|| parse_args(args, argc, argv)
		|| !(philo = malloc(sizeof(t_two))))
		return (free_philosophers(&args, philo, &forks, EXIT_FAILURE));
	sem_unlink("/forks");
	forks = sem_open("/forks", O_CREAT, 0660, args->number_of_philosophers);
	if (forks == SEM_FAILED
		|| recruit_philosophers(args, &philo, forks, 1)
		|| start_simulation(philo, args))
		return (free_philosophers(&args, philo, &forks, EXIT_FAILURE));
	wait_till_the_end(philo, args, args->start_time);
	return (free_philosophers(&args, philo, &forks, EXIT_SUCCESS));
}
