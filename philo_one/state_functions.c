/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:14:57 by user42            #+#    #+#             */
/*   Updated: 2020/12/21 22:49:56 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_one.h"

static int		think(t_one *philo, int *index, pthread_mutex_t **lock)
{
	print_activity(get_time() - philo->args->start_time, *index, THINK, *lock);
	if (!philo->eaten_meals && !(philo->index % 2))
		usleep(1000 * philo->args->time_to_eat);
	if (philo && philo->state)
		philo->state = TAKING_FORK;
	if (!philo)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int		take_fork(t_one *philo, int *index, pthread_mutex_t **lock)
{
	pthread_mutex_lock(philo->right_fork);
	print_activity(get_time() - philo->args->start_time, *index, FORKL, *lock);
	pthread_mutex_lock(philo->left_fork);
	print_activity(get_time() - philo->args->start_time, *index, FORKR, *lock);
	if (philo->state)
		philo->state = EATING;
	return (EXIT_SUCCESS);
}

static int		eat(t_one *philo, int *index, pthread_mutex_t **lock)
{
	size_t start;

	print_activity(get_time() - philo->args->start_time, *index, EAT, *lock);
	start = get_time();
	philo->time_of_death = get_time() + philo->args->time_to_die;
	while (philo && get_time() < (start + philo->args->time_to_eat))
		usleep(1000);
	if (!philo)
		return (EXIT_FAILURE);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	philo->eaten_meals++;
	if (philo->eaten_meals == philo->args->times_philo_must_eat)
		return (FULL);
	if (philo->state)
		philo->state = SLEEPING;
	return (EXIT_SUCCESS);
}

static int		dream(t_one *philo, int *index, pthread_mutex_t **lock)
{
	size_t start;

	print_activity(get_time() - philo->args->start_time, *index, SLEEP, *lock);
	start = get_time();
	while (philo && get_time() < (start + philo->args->time_to_sleep))
		usleep(1000);
	if (!philo)
		return (EXIT_FAILURE);
	if (philo->state)
		philo->state = THINKING;
	return (EXIT_SUCCESS);
}

void			*being_a_philosopher(void *arg)
{
	t_one				*philo;
	static t_function	life[4] = {think, take_fork, eat, dream};
	pthread_mutex_t		**lock;

	philo = (t_one *)arg;
	lock = &philo->args->print_lock;
	if (philo->index == philo->args->number_of_philosophers)
		philo->args->start_time = get_time();
	while (!philo->args->start_time)
		usleep(10);
	philo->time_of_death = get_time() + philo->args->time_to_die;
	while (philo->args->times_philo_must_eat >= -1 && philo->state
		&& !life[philo->state - 1](philo, &philo->index, lock))
		;
	return (NULL);
}
