/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:04:16 by user42            #+#    #+#             */
/*   Updated: 2021/02/16 00:52:44 by amartin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_three.h"

static void	wait_till_the_end(pid_t *pids, t_args *args)
{
	int		full_philosophers;
	int		status;
	int		i;

	full_philosophers = 0;
	i = 0;
	sem_wait(args->start_wait);
	while (full_philosophers < args->philo_count)
	{
		waitpid(-1, &status, 0);
		if ((status = WEXITSTATUS(status)) != 0)
		{
			while (i < args->philo_count)
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

void		*check_health(void *arg)
{
	t_three	*philo;
	int		i;

	philo = (t_three*)arg;
	if (philo->index == philo->args->philo_count)
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
	philo->time_of_death = philo->args->start_time + philo->args->time_to_die;
	while (get_time() <= philo->time_of_death)
		usleep(50);
	print_activity(philo->time_of_death - philo->args->start_time,
		philo->index, DIE, philo->args->lock);
	philo->state = DEAD;
	philo->args->lock = NULL;
	philo = NULL;
	return (NULL);
}

static void	init_philosopher(t_three *philo, t_args *args, int index)
{
	philo->index = index;
	philo->args = args;
	philo->eaten_meals = 0;
	philo->index = index;
	philo->state = THINKING;
	philo->start_time = 0;
	philo->time_of_death = 0;
	pthread_create(&(philo->thread), NULL, &check_health, philo);
	pthread_detach(philo->thread);
	being_a_philosopher(philo);
	if (philo->state)
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
	t_three	philo;

	sem_unlink("/start");
	args->start_wait = sem_open("/start", O_CREAT, 0660, 0);
	index = 1;
	args->start_time = get_time();
	while (index <= args->philo_count)
	{
		if ((pid = fork()) == -1)
			return (EXIT_FAILURE);
		else if (pid == 0)
		{
			free(pids);
			init_philosopher(&philo, args, index);
		}
		else
			pids[index - 1] = pid;
		index++;
	}
	return (EXIT_SUCCESS);
}

int			main(int argc, char **argv)
{
	t_args			arg;
	pid_t			*pids;
	sem_t			*print;

	pids = NULL;
	memset(&arg, 0, sizeof(t_args));
	if (parse_args(&arg, argc, argv)
		|| (!(pids = malloc(sizeof(pid_t) * arg.philo_count))))
		return (EXIT_FAILURE);
	sem_unlink("/forks");
	sem_unlink("/lock");
	arg.fork_pairs = sem_open("/forks", O_CREAT, 0660, arg.philo_count / 2);
	print = sem_open("/lock", O_CREAT, 0660, 1);
	arg.lock = &print;
	if (arg.fork_pairs == SEM_FAILED || print == SEM_FAILED
		|| recruit_philosophers(&arg, pids))
	{
		free(pids);
		return (EXIT_FAILURE);
	}
	wait_till_the_end(pids, &arg);
	sem_close(arg.fork_pairs);
	sem_close(print);
	free(pids);
	return (EXIT_SUCCESS);
}
