/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:14:57 by user42            #+#    #+#             */
/*   Updated: 2021/02/16 20:10:37 by amartin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_one.h"

static int		think(t_one *philo, int index, pthread_mutex_t *lok)
{
	if (philo->args->times_must_eat != -2)
		print_activity(get_time() - philo->args->start_time, index, THINK, lok);
	if (philo->index % 2 && philo->args->number_of_philosophers != 1
		&& !philo->eaten_meals)
	{
		philo->time_activity_end = get_time() + philo->args->time_to_eat;
		while (get_time() < philo->time_activity_end)
			usleep(50);
	}
	if (philo && philo->state)
		philo->state = TAKING_FORK;
	if (!philo || philo->args->times_must_eat == -2)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int		take_fork(t_one *philo, int index, pthread_mutex_t *lock)
{
	if (philo->args->times_must_eat == -2)
		return (EXIT_FAILURE);
	pthread_mutex_lock(philo->left_fork);
	if (philo->args->times_must_eat != -2)
		print_activity(get_time() - philo->args->start_time, index, FORK, lock);
	pthread_mutex_lock(philo->right_fork);
	if (philo->args->times_must_eat != -2)
		print_activity(get_time() - philo->args->start_time, index, FORK, lock);
	if (philo->state)
		philo->state = EATING;
	if (!philo || philo->args->times_must_eat == -2)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int		eat(t_one *philo, int index, pthread_mutex_t *lock)
{
	if (philo->args->times_must_eat != -2)
		print_activity(get_time() - philo->args->start_time, index, EAT, lock);
	philo->time_of_death = get_time() + philo->args->time_to_die;
	philo->time_activity_end = get_time() + philo->args->time_to_eat;
	while (get_time() < philo->time_activity_end)
		usleep(50);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	if (!philo || philo->args->times_must_eat == -2)
		return (EXIT_FAILURE);
	philo->eaten_meals++;
	if (philo->eaten_meals == philo->args->times_must_eat)
		return (FULL);
	if (philo->state)
		philo->state = SLEEPING;
	return (EXIT_SUCCESS);
}

static int		dream(t_one *philo, int index, pthread_mutex_t *lok)
{
	if (philo->args->times_must_eat != -2)
		print_activity(get_time() - philo->args->start_time, index, SLEEP, lok);
	philo->time_activity_end = get_time() + philo->args->time_to_sleep;
	while (get_time() < philo->time_activity_end)
		usleep(50);
	if (!philo || philo->args->times_must_eat == -2)
		return (EXIT_FAILURE);
	if (philo->state)
		philo->state = THINKING;
	return (EXIT_SUCCESS);
}

void			*being_a_philosopher(void *arg)
{
	t_one				*philo;
	static t_function	life[4] = {think, take_fork, eat, dream};
	pthread_mutex_t		*lock;

	philo = (t_one *)arg;
	lock = philo->args->print_lock;
	if (philo->index == philo->args->number_of_philosophers)
		philo->args->start_time = get_time();
	while (!philo->args->start_time)
		usleep(50);
	philo->time_of_death = get_time() + philo->args->time_to_die;
	while (philo && philo->args->times_must_eat >= -1 && philo->state
		&& !life[philo->state - 1](philo, philo->index, lock))
		usleep(50);
	return (NULL);
}
