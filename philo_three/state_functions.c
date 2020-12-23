/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:14:57 by user42            #+#    #+#             */
/*   Updated: 2020/12/11 17:39:32 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_three.h"

int		take_fork(t_three *philo, int index, t_args *args)
{
	if (args->times_must_eat < -1 || philo->eaten_meals == args->times_must_eat
		|| philo->time_of_death < get_time())
		return (EXIT_FAILURE);
	sem_wait(args->forks);
	if (args->times_must_eat >= -1 && philo->time_of_death > get_time())
		print_activity(get_time() - args->start_time, index, FORK, args->lock);
	else
		return (sem_post(args->forks) - EXIT_FAILURE);
	sem_wait(args->forks);
	if (args->times_must_eat >= -1 && philo->time_of_death > get_time())
		print_activity(get_time() - args->start_time, index, FORK, args->lock);
	else
		return (sem_post(args->forks) + sem_post(args->forks) - 1);
	return (EXIT_SUCCESS);
}

int		eat(t_three *philo, int index, t_args *args)
{
	if (args->times_must_eat < -1 || philo->eaten_meals == args->times_must_eat
		|| philo->time_of_death < get_time())
	{
		sem_post(args->forks);
		sem_post(args->forks);
		return (EXIT_FAILURE);
	}
	print_activity(get_time() - args->start_time, index, EAT, args->lock);
	philo->time_of_death = get_time() + args->time_to_die;
	usleep(1000 * args->time_to_eat);
	sem_post(args->forks);
	sem_post(args->forks);
	if (philo->eaten_meals >= 0)
		philo->eaten_meals++;
	if (philo->eaten_meals == args->times_must_eat)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int		sleep_tight(t_three *philo, int index, t_args *args)
{
	if (args->times_must_eat < -1 || philo->eaten_meals == args->times_must_eat)
		return (EXIT_FAILURE);
	if (philo->time_of_death > get_time())
		print_activity(get_time() - args->start_time, index, SLEEP, args->lock);
	usleep(1000 * args->time_to_sleep);
	return (EXIT_SUCCESS);
}

int		think(t_three *philo, int index, t_args *args)
{
	if (args->times_must_eat < -1 || philo->eaten_meals == args->times_must_eat
		|| philo->time_of_death < get_time())
		return (EXIT_FAILURE);
	else
		print_activity(get_time() - args->start_time, index, THINK, args->lock);
	return (EXIT_SUCCESS);
}

void	*being_a_philosopher(void *arg)
{
	t_three *philo;

	philo = (t_three *)arg;
	while (philo->args->times_must_eat >= -1
		&& philo->eaten_meals != philo->args->times_must_eat)
	{
		if (take_fork(philo, philo->index, philo->args)
			|| eat(philo, philo->index, philo->args)
			|| sleep_tight(philo, philo->index, philo->args)
			|| think(philo, philo->index, philo->args))
			break ;
	}
	return (NULL);
}
