/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:04:16 by user42            #+#    #+#             */
/*   Updated: 2021/02/17 19:42:17 by amartin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_three.h"

static void	wait_till_the_end(pid_t *pids, t_args *args)
{
	int		full_philosophers;
	int		status;
	int		i;

	(void)pids;
	full_philosophers = 0;
	i = 0;
//	sem_wait(args->start_wait);
	while (full_philosophers < args->philo_count)
	{
		waitpid(-1, &status, 0);
		if ((status = WEXITSTATUS(status)) != 0)
		{
			args->times_must_eat = -2;
			usleep(args->time_to_die * 1000);
/*			while (i < args->philo_count)
			{
			//	sem_post(args->fork_pairs);
			//	waitpid(pids[i], NULL, 0);
			//	(void)pids;
				kill(pids[i], SIGKILL);
				i++;
			}
*/			return ;
		}
		else
			full_philosophers++;
	}
	return ;
}

void		check_health(t_three *philo)
{
//	int		i;


/*	if (philo->index == philo->args->philo_count)
	{
		i = philo->args->philo_count + 1;
		while (i--)
			sem_post(philo->args->start_wait);
		sem_close(philo->args->start_wait);
		sem_unlink("/start");
	}
	else
		sem_wait(philo->args->start_wait);
	usleep(500);
*/	philo->time_of_death = philo->args->start_time + philo->args->time_to_die;
	while (get_time() <= philo->time_of_death
		&& philo->eaten_meals != philo->args->time_to_eat
		&& philo->args->times_must_eat >= -1)
		usleep(50);
	if (philo->eaten_meals != philo->args->time_to_eat)
	{
		philo->state = DEAD;
		philo->args->times_must_eat = -2;
		print_activity(philo->time_of_death - philo->args->start_time,
			philo->index, DIE, &(philo->args->lock));
	}
//	sem_close(philo->args->lock);
//	philo->args->lock = NULL;
//	philo = NULL;
//	return (NULL);
}

static void	init_philosopher(t_args *args, int index)
{
	t_three	philo;

	philo.index = index;
	philo.args = args;
	philo.eaten_meals = 0;
	philo.index = index;
	philo.state = THINKING;
	philo.start_time = 0;
	philo.time_of_death = 0;
	pthread_create(&(philo.thread), NULL, &being_a_philosopher, &philo);
	check_health(&philo);
	pthread_join(philo.thread, NULL);
	if (philo.state)
		exit(0);
	else
		exit(index);
}

/*
** fork return values:
**    -1: error
**     0: you're in a child process
** other: you're in the parent process, the return value is the child process ID
*/

static int	recruit_philosophers(t_args *args, pid_t *pids)
{
	pid_t	pid;
	int		index;

	index = 1;
	args->start_time = get_time();
	while (index <= args->philo_count)
	{
		if ((pid = fork()) == -1)
			return (EXIT_FAILURE);
		else if (pid == 0)
		{
			free(pids);
			init_philosopher(args, index);
		}
		else
			pids[index - 1] = pid;
		index++;
	}
	if (pid)
		wait_till_the_end(pids, args);
	return (EXIT_SUCCESS);
}

int			main(int argc, char **argv)
{
	t_args			*args;
	pid_t			*pids;

	if (!(args = malloc(sizeof(t_args))))
		return (EXIT_FAILURE);
	if (parse_args(args, argc, argv)
		|| (!(pids = malloc(sizeof(pid_t) * args->philo_count))))
		return (free_philosophers(args, NULL, EXIT_FAILURE));
	sem_unlink("/forks");
	sem_unlink("/lock");
	args->fork_pairs = sem_open("/forks", O_CREAT, 0660, args->philo_count / 2);
	args->lock = sem_open("/lock", O_CREAT, 0660, 1);
	if (args->fork_pairs == SEM_FAILED || args->lock == SEM_FAILED
		|| recruit_philosophers(args, pids))
		return (free_philosophers(args, pids, EXIT_FAILURE));
	return (free_philosophers(args, pids, EXIT_SUCCESS));
}
