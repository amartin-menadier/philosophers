/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:14:57 by user42            #+#    #+#             */
/*   Updated: 2020/12/11 12:46:47 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_one.h"

int		take_fork(t_one *philo, int index)
{
	if (philo->args->times_philo_must_eat < -1
		|| philo->eaten_meals == philo->args->times_philo_must_eat
		|| philo->time_of_death < get_time())
		return (EXIT_FAILURE);
	pthread_mutex_lock(philo->left_fork);
	if (philo->args->times_philo_must_eat >= -1
		&& philo->time_of_death > get_time())
		print_activity(get_time() - philo->args->start_time, index, FORK);
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		return (EXIT_FAILURE);
	}
	pthread_mutex_lock(philo->right_fork);
	if (philo->args->times_philo_must_eat >= -1
		&& philo->time_of_death > get_time())
		print_activity(get_time() - philo->args->start_time, index, FORK);
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int		eat(t_one *philo, t_args *args, int index)
{
	size_t	time_to_eat_and_die;

	if (philo->args->times_philo_must_eat < -1
		|| philo->eaten_meals == philo->args->times_philo_must_eat)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (EXIT_FAILURE);
	}
	time_to_eat_and_die = (args->time_to_eat + args->time_to_die) * 1000;
	philo->time_of_death = get_time() + time_to_eat_and_die;
	if (philo->time_of_death > get_time())
		print_activity(get_time() - args->start_time, index, EAT);
	usleep(999 * args->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	if (philo->eaten_meals >= 0)
		philo->eaten_meals++;
	if (philo->eaten_meals == philo->args->times_philo_must_eat)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int		sleep_tight(t_one *philo, int index)
{
	if (philo->args->times_philo_must_eat < -1
		|| philo->eaten_meals == philo->args->times_philo_must_eat)
		return (EXIT_FAILURE);
	if (philo->time_of_death > get_time())
		print_activity(get_time() - philo->args->start_time, index, SLEEP);
	usleep(999 * philo->args->time_to_sleep);
	return (EXIT_SUCCESS);
}

int		think(t_one *philo, int index)
{
	if (philo->args->times_philo_must_eat < -1
		|| philo->eaten_meals == philo->args->times_philo_must_eat
		|| philo->time_of_death < get_time())
		return (EXIT_FAILURE);
	else
		print_activity(get_time() - philo->args->start_time, index, THINK);
	return (EXIT_SUCCESS);
}

void	*being_a_philosopher(void *arg)
{
	t_one *philo;

	philo = (t_one *)arg;
	while (philo->args->times_philo_must_eat >= -1
		&& philo->eaten_meals != philo->args->times_philo_must_eat)
	{
		if (take_fork(philo, philo->index)
			|| eat(philo, philo->args, philo->index)
			|| sleep_tight(philo, philo->index)
			|| think(philo, philo->index))
			break ;
	}
	return (NULL);
}
