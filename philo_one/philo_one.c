/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:04:16 by user42            #+#    #+#             */
/*   Updated: 2020/12/07 16:29:51 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_one.h"

int		recruit_philosophers(t_args *args, t_philo **philo, size_t index)
{
	if (index != 1 && !((*philo) = (t_philo *)malloc(sizeof(t_philo))))
		return (EXIT_FAILURE);
	(*philo)->args = args;
	(*philo)->index = index;
	(*philo)->thread = -1;
	(*philo)->time_of_death = 0;
	(*philo)->right_fork = NULL;
	(*philo)->eaten_meals = 0;
	if (!((*philo)->left_fork = malloc(sizeof(pthread_mutex_t))))
		return (EXIT_FAILURE);
	pthread_mutex_init((*philo)->left_fork, NULL);
	if (index == args->number_of_philosophers)
		(*philo)->next = NULL;
	else
		return (recruit_philosophers(args, &(*philo)->next, index + 1));
	return (EXIT_SUCCESS);
}

int		link_forks(t_philo *philo, pthread_mutex_t *right_fork, int index)
{
	t_philo	*last;

	last = philo;
	while (index == 1 && last && last->next)
		last = last->next;
	if (philo->args->number_of_philosophers == 1)
	{
		if (!(philo->right_fork = malloc(sizeof(pthread_mutex_t))))
			return (EXIT_FAILURE);
		pthread_mutex_init(philo->right_fork, NULL);
	}
	else if (index == 1)
		philo->right_fork = last->left_fork;
	else
		philo->right_fork = right_fork;
	if (philo->next)
		return (link_forks(philo->next, philo->left_fork, index + 1));
	return (EXIT_SUCCESS);
}

int		start_simulation(t_philo *philo, t_args *args)
{
	if (!philo)
		return (EXIT_SUCCESS);
	if (philo->index == 1)
		args->start_time = get_time();
	philo->time_of_death = args->start_time + args->time_to_die * 1000;
	pthread_create(&philo->thread, NULL, being_a_philosopher, philo);
	return (start_simulation(philo->next, args));
}

void	wait_till_the_end(t_philo *philo, t_args *args, size_t start_time)
{
	t_philo	*first;
	size_t	full_philosophers;

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
	else
		print_activity(get_time() - start_time, 0, YUMMY);
	return ;
}

int		main(int argc, char **argv)
{
	t_args			*args;
	t_philo			*philo;

	args = NULL;
	philo = NULL;
	write(1, "\n\nSTARTING SIMULATION\n\n", 24);
	if (!(args = malloc(sizeof(t_args)))
		|| !(philo = malloc(sizeof(t_philo)))
		|| parse_args(args, argc, argv)
		|| recruit_philosophers(args, &philo, 1)
		|| link_forks(philo, NULL, 1)
		|| start_simulation(philo, args))
		return (free_philosophers(&args, &philo, EXIT_FAILURE));
	wait_till_the_end(philo, args, args->start_time);
	return (free_philosophers(&args, &philo, EXIT_SUCCESS));
}
