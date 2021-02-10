/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:04:16 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 00:20:51 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_one.h"

static int	recruit_philosophers(t_args *args, t_one *philo, int index)
{
	philo->args = args;
	philo->state = THINKING;
	philo->index = index;
	philo->time_of_death = 0;
	philo->time_activity_end = 0;
	philo->right_fork = NULL;
	philo->eaten_meals = 0;
	if (!(philo->left_fork = malloc(sizeof(pthread_mutex_t)))
		|| pthread_mutex_init(philo->left_fork, NULL))
		return (EXIT_FAILURE);
	if (index == args->number_of_philosophers)
		philo->next = NULL;
	else if (!(philo->next = (t_one *)malloc(sizeof(t_one))))
		return (EXIT_FAILURE);
	else
		return (recruit_philosophers(args, philo->next, index + 1));
	return (EXIT_SUCCESS);
}

static int	link_forks(t_one *philo, pthread_mutex_t *right_fork, int index)
{
	t_one	*last;

	last = philo;
	while (index == 1 && last->next)
		last = last->next;
	if (index == 1)
		philo->right_fork = last->left_fork;
	else
		philo->right_fork = right_fork;
	if (philo->next)
		return (link_forks(philo->next, philo->left_fork, index + 1));
	return (EXIT_SUCCESS);
}

static int	start_simulation(t_one *philo, t_args *args)
{
	if (!philo)
		return (EXIT_SUCCESS);
	pthread_create(&philo->thread, NULL, being_a_philosopher, philo);
	return (start_simulation(philo->next, args));
}

static void	wait_till_the_end(t_one *philo, t_args *args, pthread_mutex_t *lock)
{
	t_one	*first;
	int		full_philosophers;

	first = philo;
	full_philosophers = 0;
	usleep(args->time_to_die * 1000);
	while (get_time() < philo->time_of_death)
	{
		if (philo->eaten_meals == args->times_must_eat)
			full_philosophers++;
		if (philo->next)
			philo = philo->next;
		else if (full_philosophers != args->number_of_philosophers)
		{
			philo = first;
			full_philosophers = 0;
		}
		else
			break ;
	}
	args->times_must_eat = -2;
	if (full_philosophers != args->number_of_philosophers)
		print_activity(get_time() - args->start_time, philo->index, DIE, lock);
	pthread_detach(philo->thread);
	return ;
}

int			main(int argc, char **argv)
{
	t_args	*args;
	t_one	*philo;

	args = NULL;
	philo = NULL;
	if (!(args = malloc(sizeof(t_args)))
		|| parse_args(args, argc, argv)
		|| !(args->print_lock = malloc(sizeof(pthread_mutex_t)))
		|| pthread_mutex_init(args->print_lock, NULL)
		|| !(philo = malloc(sizeof(t_one)))
		|| recruit_philosophers(args, philo, 1)
		|| link_forks(philo, NULL, 1)
		|| start_simulation(philo, args))
		return (free_philosophers(&args, philo, EXIT_FAILURE));
	wait_till_the_end(philo, args, args->print_lock);
	return (free_philosophers(&args, philo, EXIT_SUCCESS));
}
