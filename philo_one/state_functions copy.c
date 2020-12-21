/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_functions copy.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:14:57 by user42            #+#    #+#             */
/*   Updated: 2020/12/21 21:56:40 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_one.h"

static int		take_fork(t_one *philo, int *index)
{
	pthread_mutex_lock(philo->left_fork);
	if (philo->args->times_philo_must_eat < -1
		|| philo->time_of_death < get_time())
		return (EXIT_FAILURE);
	print_activity(get_time() - philo->args->start_time, *index, FORKL);
	pthread_mutex_lock(philo->right_fork);
	if (philo->args->times_philo_must_eat < -1
		|| philo->time_of_death < get_time())
		return (EXIT_FAILURE);
	print_activity(get_time() - philo->args->start_time, *index, FORKR);
	return (EXIT_SUCCESS);
}

static int		eat(t_one *philo, t_args *args, int *index)
{
	if (philo->args->times_philo_must_eat < -1
		|| philo->time_of_death <= get_time())
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (EXIT_FAILURE);
	}
	print_activity(get_time() - args->start_time, *index, EAT);
	philo->time_of_death = get_time() + args->time_to_die;
	usleep(999 * args->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	philo->eaten_meals++;
	if (philo->eaten_meals == philo->args->times_philo_must_eat)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int		sleep(t_one *philo, int *index)
{
	if (philo->args->times_philo_must_eat < -1)
		return (EXIT_FAILURE);
	if (philo->time_of_death > get_time())
		print_activity(get_time() - philo->args->start_time, *index, SLEEP);
	usleep(999 * philo->args->time_to_sleep);
	return (EXIT_SUCCESS);
}

static int		think(t_one *philo, int *index)
{
	if (philo->args->times_philo_must_eat < -1
		|| philo->time_of_death < get_time())
		return (EXIT_FAILURE);
	else
		print_activity(get_time() - philo->args->start_time, *index, THINK);
	if (philo->index % 2 && !philo->eaten_meals
		&& (*index != 1 || !(philo->args->number_of_philosophers % 2)))
		usleep(998 * philo->args->time_to_eat);
	return (EXIT_SUCCESS);
}

void			*being_a_philosopher(void *arg)
{
	t_one 				*philo;
	static t_function	process[4] = {think, take_fork, eat, sleep};

	philo = (t_one *)arg;
	while (philo->args->times_philo_must_eat >= -1)
	{
		if (think(philo, &philo->index)
			|| take_fork(philo, &philo->index)
			|| eat(philo, philo->args, &philo->index)
			|| sleep_tight(philo, &philo->index))
			break ;
	}
	return (NULL);
}
