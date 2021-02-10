/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:14:57 by user42            #+#    #+#             */
/*   Updated: 2020/12/31 14:55:24 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_three.h"

static int		think(t_three *philo, int *index, sem_t **lock)
{
	print_activity(get_time() - philo->args->start_time, *index, THINK, lock);
	if (philo->args->number_of_philosophers % 2 && (philo->eaten_meals
		|| (philo->index % 2 && philo->args->number_of_philosophers != 1)))
		usleep(1000 * philo->args->time_to_eat);
	if (philo && philo->state)
		philo->state = TAKING_FORK;
	if (!philo)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int		take_fork(t_three *philo, int *index, sem_t **lock)
{
	sem_wait(philo->args->forks);
	print_activity(get_time() - philo->args->start_time, *index, FORK, lock);
	sem_wait(philo->args->forks);
	print_activity(get_time() - philo->args->start_time, *index, FORK, lock);
	if (!philo)
		return (EXIT_FAILURE);
	if (philo->state)
		philo->state = EATING;
	return (EXIT_SUCCESS);
}

static int		eat(t_three *philo, int *index, sem_t **lock)
{
	philo->time_of_death = get_time() + philo->args->time_to_die;
	print_activity(get_time() - philo->args->start_time, *index, EAT, lock);
	usleep(1000 * philo->args->time_to_eat);
	if (!philo)
		return (EXIT_FAILURE);
	sem_post(philo->args->forks);
	sem_post(philo->args->forks);
	philo->eaten_meals++;
	if (philo->eaten_meals == philo->args->times_must_eat)
		return (FULL);
	if (philo->state)
		philo->state = SLEEPING;
	return (EXIT_SUCCESS);
}

static int		dream(t_three *philo, int *index, sem_t **lock)
{
	print_activity(get_time() - philo->args->start_time, *index, SLEEP, lock);
	usleep(1000 * philo->args->time_to_sleep);
	if (!philo)
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
	while (philo && philo->args->times_must_eat >= -1 && philo->state
		&& !life[philo->state - 1](philo, &philo->index, lock))
		;
	return (NULL);
}
