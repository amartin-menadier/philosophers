/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:04:16 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 17:04:59 by amartin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_two.h"

static int	free_philosophers(t_args **args, t_two *philo, int ret)
{
	if (philo)
	{
		philo->state = DEAD;
		free_philosophers(args, philo->next, ret);
	}
	if (philo && philo->thread)
	{
		pthread_detach(philo->thread);
		philo->thread = 0;
	}
	if (philo && philo->index == 1)
	{
		sem_close((*args)->fork_pairs);
		sem_unlink("forks");
		sem_close((*args)->lock);
		sem_unlink("/lock");
		free(*args);
	}
	if (philo)
		free(philo);
	philo = NULL;
	return (ret);
}

static int	start_simulation(t_two *philo, t_args *args)
{
	if (!philo)
		return (EXIT_SUCCESS);
	philo->time_of_death = get_time() + args->time_to_die;
	pthread_create(&philo->thread, NULL, being_a_philosopher, philo);
	return (start_simulation(philo->next, args));
}

static void	wait_till_the_end(t_two *philo, t_args *args, sem_t *lock)
{
	t_two	*first;
	int		full_philosophers;

	first = philo;
	full_philosophers = 0;
	usleep(50);
	while (get_time() < philo->time_of_death)
	{
		if (philo->eaten_meals == args->times_must_eat)
			full_philosophers++;
		if (philo->next)
			philo = philo->next;
		else if (full_philosophers != args->philo_count)
		{
			philo = first;
			full_philosophers = 0;
		}
		else
			break ;
	}
	args->times_must_eat = -2;
	if (full_philosophers != args->philo_count)
		print_activity(get_time() - args->start_time, philo->index, DIE, lock);
	pthread_detach(philo->thread);
	return ;
}

static int	recruit_philosophers(t_args *args, t_two **philo, int index)
{
	if (index != 1 && !((*philo) = (t_two *)malloc(sizeof(t_two))))
		return (EXIT_FAILURE);
	(*philo)->args = args;
	(*philo)->index = index;
	(*philo)->state = THINKING;
	(*philo)->time_of_death = 0;
	(*philo)->eaten_meals = 0;
	if (index == args->philo_count)
		(*philo)->next = NULL;
	else
		return (recruit_philosophers(args, &(*philo)->next, index + 1));
	return (EXIT_SUCCESS);
}

int			main(int argc, char **argv)
{
	t_args	*arg;
	t_two	*philo;

	arg = NULL;
	philo = NULL;
	if (!(arg = malloc(sizeof(t_args)))
		|| parse_args(arg, argc, argv)
		|| !(philo = malloc(sizeof(t_two))))
		return (free_philosophers(&arg, philo, EXIT_FAILURE));
	philo->next = NULL;
	philo->index = 1;
	sem_unlink("forks");
	sem_unlink("/lock");
	arg->fork_pairs = sem_open("forks", O_CREAT, 0660, arg->philo_count / 2);
	arg->lock = sem_open("/lock", O_CREAT, 0660, 1);
	if (arg->fork_pairs == SEM_FAILED || arg->lock == SEM_FAILED
		|| recruit_philosophers(arg, &philo, 1)
		|| start_simulation(philo, arg))
		return (free_philosophers(&arg, philo, EXIT_FAILURE));
	wait_till_the_end(philo, arg, arg->lock);
	return (free_philosophers(&arg, philo, EXIT_SUCCESS));
}
