/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:04:16 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 21:57:47 by amartin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_two.h"

static int	free_philosophers(t_args *args, t_two **philo, int ret)
{
	write(1, "Starting to free\n", 18);
	if ((*philo)->next)
		free_philosophers(args, &(*philo)->next, ret);
//	write(1, "Detaching thread\n", 18);
	pthread_join((*philo)->thread, NULL);
	(*philo)->thread = 0;		
	if (*philo && (*philo)->index == 1 && args)
	{
		write(1, "Sem_close_lock\n", 16);
		sem_close(args->lock);
		write(1, "Unlink lock\n", 13);
		sem_unlink("/lock");
		
		write(1, "sem_close_forks\n", 17);
		sem_close(args->fork_pairs);
		write(1, "Unlink forks\n", 14);
		sem_unlink("/forks");
		
		write(1, "Free args\n", 11);
		free(args);
	}
	free (*philo);
	*philo = NULL;
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
//	usleep(50);
	while (full_philosophers < args->philo_count)
	{
		if (philo->state && philo->eaten_meals == args->times_must_eat)
		{
			philo->state = DEAD;
			full_philosophers++;
		}
		if (get_time() > philo->time_of_death
			&& philo->eaten_meals != args->times_must_eat)
			break ;
		if (philo->next)
			philo = philo->next;
		else
			philo = first;
	}
	args->times_must_eat = -2;
	if (full_philosophers != args->philo_count)
		print_activity(get_time() - args->start_time, philo->index, DIE, lock);
	usleep(200);
//	pthread_detach(philo->thread);
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
		return (free_philosophers(arg, &philo, EXIT_FAILURE));
	philo->next = NULL;
	philo->index = 1;
	sem_unlink("/forks");
	sem_unlink("/lock");
	arg->fork_pairs = sem_open("/forks", O_CREAT, 0660, arg->philo_count / 2);
	arg->lock = sem_open("/lock", O_CREAT, 0660, 1);
	if (arg->fork_pairs == SEM_FAILED || arg->lock == SEM_FAILED
		|| recruit_philosophers(arg, &philo, 1)
		|| start_simulation(philo, arg))
		return (free_philosophers(arg, &philo, EXIT_FAILURE));
	wait_till_the_end(philo, arg, arg->lock);
	return (free_philosophers(arg, &philo, EXIT_SUCCESS));
}
