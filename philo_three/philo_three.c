/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:04:16 by user42            #+#    #+#             */
/*   Updated: 2020/12/11 14:35:27 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_three.h"

int		free_philosophers(t_args **args, pid_t **pids, int ret)
{
	int	i;

	i = 0;
	while ((*pids) && i < (*args)->number_of_philosophers)
	{
		kill((*pids)[i], SIGKILL);
		i++;
	}
	sem_close((*args)->forks);
	sem_unlink("/forks");
	sem_close((*args)->lock);
	sem_unlink("/lock");
	free(*args);
	(*args) = NULL;
	free(*pids);
	*pids = NULL;
	return (ret);
}

/*
** start_simulation (= philosopher process main function) returns the
** philosopher's number/index if she is dead, else 0 (all meals eaten)
*/

int		start_simulation(t_args *args, size_t index)
{
	t_three	philo;

	memset(&philo, 0, sizeof(t_three));
	philo.args = args;
	philo.eaten_meals = 0;
	philo.index = index;
	philo.time_of_death = args->start_time + args->time_to_die;
	pthread_create(&(philo.thread), NULL, being_a_philosopher, &philo);
	usleep(1000);
	while (philo.time_of_death > get_time()
		&& (args->times_must_eat >= -1
		&& philo.eaten_meals != args->times_must_eat))
		;
	pthread_detach(philo.thread);
	if (philo.eaten_meals == args->times_must_eat)
		exit(0);
	print_activity(philo.time_of_death - args->start_time,
		index, DIE, args->lock);
	exit(index);
}

void	wait_till_the_end(pid_t *pids, t_args *args)
{
	int		full_philosophers;
	int		status;
	int		i;

	full_philosophers = 0;
	i = 0;
	while (full_philosophers < args->number_of_philosophers)
	{
		waitpid(-1, &status, 0);
		if ((status = WEXITSTATUS(status)) != 0)
		{
			while (i < args->number_of_philosophers)
			{
				kill(pids[i], SIGKILL);
				i++;
			}
			return ;
		}
		else
			full_philosophers++;
	}
	return ;
}

/*
** fork return values:
**    -1: error
**     0: you're in a child process
** other: you're in the parent process, the return value is the child process ID
*/

int		recruit_philosophers(t_args *args, pid_t **pids)
{
	pid_t	pid;
	int		index;

	index = 1;
	args->start_time = get_time();
	while (index <= args->number_of_philosophers)
	{
		if ((pid = fork()) == -1)
			return (EXIT_FAILURE);
		else if (pid == 0)
			start_simulation(args, index);
		else
			(*pids)[index - 1] = pid;
		index++;
	}
	return (EXIT_SUCCESS);
}

/*
** lock[0] aka "/forks" is used for the forks ;
** lock[1] aka "/print" is used to prevent 1. simultaneous uses of write by
** different processes/philosophers or 2. any printing after a philosopher died.
*/

int		main(int argc, char **argv)
{
	t_args			*arg;
	pid_t			*pids;

	arg = NULL;
	pids = NULL;
	if (!(arg = malloc(sizeof(t_args)))
		|| parse_args(arg, argc, argv)
		|| (!(pids = malloc(sizeof(pid_t) * arg->number_of_philosophers))))
		return (free_philosophers(&arg, &pids, EXIT_FAILURE));
	sem_unlink("/forks");
	sem_unlink("/lock");
	arg->forks = sem_open("/forks", O_CREAT, 0660, arg->number_of_philosophers);
	arg->lock = sem_open("/lock", O_CREAT, 0660, 1);
	if (arg->forks == SEM_FAILED || arg->lock == SEM_FAILED
		|| recruit_philosophers(arg, &pids))
		return (free_philosophers(&arg, &pids, EXIT_FAILURE));
	wait_till_the_end(pids, arg);
	return (free_philosophers(&arg, &pids, EXIT_SUCCESS));
}
