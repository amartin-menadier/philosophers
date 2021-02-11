/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:14:57 by user42            #+#    #+#             */
/*   Updated: 2021/02/11 16:23:26 by amartin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_three.h"

static int		think(t_three *philo, int *index, sem_t **lock)
{
	size_t	activity_end;

	print_activity(get_time() - philo->args->start_time, *index, THINK, lock);
	if (philo->index % 2 && philo->args->philo_count != 1
		&& !philo->eaten_meals)
	{
		activity_end = get_time() + philo->args->time_to_eat;
		while (philo && get_time() < activity_end)
			usleep(50);
	}
	if (philo && philo->state)
		philo->state = TAKING_FORK;
	if (!philo)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int		take_fork(t_three *philo, int *index, sem_t **lock)
{
	sem_wait(philo->args->fork_pairs);
	if (philo->args->times_must_eat < -1)
		return (EXIT_FAILURE);
	print_activity(get_time() - philo->args->start_time, *index, FORK, lock);
	print_activity(get_time() - philo->args->start_time, *index, FORK, lock);
	if (philo->state)
		philo->state = EATING;
	return (EXIT_SUCCESS);
}

static int		eat(t_three *philo, int *index, sem_t **lock)
{
	size_t	activity_end;

	print_activity(get_time() - philo->args->start_time, *index, EAT, lock);
	activity_end = get_time() + philo->args->time_to_eat;
	philo->time_of_death = get_time() + philo->args->time_to_die;
	while (philo->args->times_must_eat >= -1 && get_time() < activity_end)
		usleep(50);
	sem_post(philo->args->fork_pairs);
	if (philo->args->times_must_eat < -1)
		return (EXIT_FAILURE);
	philo->eaten_meals++;
	if (philo->eaten_meals == philo->args->times_must_eat)
		return (FULL);
	if (philo->state)
		philo->state = SLEEPING;
	return (EXIT_SUCCESS);
}

static int		dream(t_three *philo, int *index, sem_t **lock)
{
	size_t	activity_end;

	print_activity(get_time() - philo->args->start_time, *index, SLEEP, lock);
	activity_end = get_time() + philo->args->time_to_sleep;
	while (philo->args->times_must_eat >= -1 && get_time() < activity_end)
		usleep(50);
	if (philo->args->times_must_eat < -1)
		return (EXIT_FAILURE);
	if (philo->state)
		philo->state = THINKING;
	return (EXIT_SUCCESS);
}

void			*being_a_philosopher(void *arg)
{
	t_three				*philo;
	static t_function	life[4] = {think, take_fork, eat, dream};
	sem_t				**lock;

	philo = (t_three *)arg;
	lock = philo->args->lock;
	philo->args->start_time = get_time();
	philo->time_of_death = get_time() + philo->args->time_to_die;
	while (philo && philo->args->times_must_eat >= -1 && philo->state
		&& !life[philo->state - 1](philo, &philo->index, lock))
		usleep(50);
	return (NULL);
}
