/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:04:16 by user42            #+#    #+#             */
/*   Updated: 2020/12/04 22:24:40 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_one.h"

char	*get_msg(char *time, char *index, char *activity)
{
	char	*msg;
	int		len;
	int		i;

	len = basic_strlen(time) + basic_strlen(index) + basic_strlen(activity) + 3;
	if (!(msg = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	msg[len] = '\0';
	i = 0;
	while (i < len && time && *time)
		msg[i++] = *time++;
	msg[i++] = ' ';
	while (i < len && index && *index)
		msg[i++] = *index++;
	msg[i++] = ' ';
	while (i < len && activity && *activity)
		msg[i++] = *activity++;
	msg[i] = '\n';
	return (msg);
}

void	print_activity(size_t timestamp, t_philo *philo, char *activity)
{
	char	*tmp;
	char	*time;
	char	*index;

	timestamp /= 1000;
	time = basic_itoa(timestamp);
	index = basic_itoa((size_t)philo->index);
	tmp = get_msg(time, index, activity);
	write(1, tmp, basic_strlen(tmp));
	free(time);
	free(index);
	free(tmp);
}

int		take_fork(t_philo *philo)
{
	if (!philo->index)
		return (EXIT_FAILURE);
	pthread_mutex_lock(philo->left_fork);
	print_activity(get_time() - philo->args->start_time, philo, FORK);
	pthread_mutex_lock(philo->right_fork);
	print_activity(get_time() - philo->args->start_time, philo, FORK);
	return (EXIT_SUCCESS);
}

int		eat(t_philo *philo, t_args *args)
{
	if (!philo->index)
		return (EXIT_FAILURE);
	philo->time_of_death = get_time() + args->time_to_eat * 1000 + args->time_to_die * 1000;
	print_activity(get_time() - philo->args->start_time, philo, EAT);
	usleep(1000 * philo->args->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (EXIT_SUCCESS);
}

int 	sleep_tight(t_philo *philo)
{
	if (!philo->index)
		return (EXIT_FAILURE);
	print_activity(get_time() - philo->args->start_time, philo, SLEEP);
	usleep(1000 * philo->args->time_to_sleep);
	return (EXIT_SUCCESS);
}

int 	think(t_philo *philo)
{
	if (!philo->index)
		return (EXIT_FAILURE);
	print_activity(get_time() - philo->args->start_time, philo, THINK);
	return (EXIT_SUCCESS);
}

void	*being_a_philosophe(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
//	basic_putstr("being a philosophe index = \n", philo->index, NULL, 0);
	while (philo->index)
	{
//		basic_putstr("being a philosophe inside 1\n", 0, NULL, 0);
		if (take_fork(philo)
			|| eat(philo, philo->args)
			|| sleep_tight(philo)
			|| think(philo))
			break;
	}
//	print_activity(get_time() - philo->args->start_time, philo, NULL);
	return (NULL);
}

int		start_simulation(t_philo *philo)
{
	if (!philo)
		return (EXIT_SUCCESS);
	if (philo->index == 1)
		philo->args->start_time = get_time();
	philo->time_of_death = philo->args->start_time + philo->args->time_to_die * 1000;
	pthread_create(&philo->thread, NULL, being_a_philosophe, philo);
	return (start_simulation(philo->next));
}

void	wait_till_the_end(t_philo *philo)
{
	t_philo	*first;
	
	first = philo;
	while (get_time() < philo->time_of_death)
	{
		if (philo->next)
			philo = philo->next;
		else
			philo = first;
	}
	print_activity(philo->time_of_death - philo->args->start_time, philo, DIE);
	philo->index = 0;
	return ;
}

int		free_philosophers(t_args **args, t_philo **philo, int ret)
{
	if (args && *args)
	{
		if ((*args)->number_of_philosophers == 1)
		{
			pthread_mutex_destroy((*philo)->right_fork);
			free((*philo)->right_fork);
		}
		free(*args);
		(*args) = NULL;
	}
	if ((*philo)->next)
		free_philosophers(NULL, &((*philo)->next), ret);
	if ((*philo)->left_fork)
	{
		pthread_mutex_destroy((*philo)->left_fork);
		free((*philo)->left_fork);
	}
	if ((*philo)->thread)
	{
		pthread_detach((*philo)->thread);
		(*philo)->thread = 0;
	}
	free(*philo);
	*philo = NULL;
	return (ret);
}

int		recruit_philosophers(t_args *args, t_philo **philo, size_t index)
{
	if (index != 1 && !((*philo) = (t_philo *)malloc(sizeof(t_philo))))
		return (EXIT_FAILURE);
	(*philo)->args = args;
	(*philo)->index = index;
	(*philo)->thread = -1;
	(*philo)->time_of_death = 0;
	(*philo)->right_fork = NULL;
	if (!((*philo)->left_fork = malloc(sizeof(pthread_mutex_t))))
		return (EXIT_FAILURE);
	pthread_mutex_init((*philo)->left_fork, NULL);
	if (index == args->number_of_philosophers)
		(*philo)->next = NULL;
	else
		return (recruit_philosophers(args, &(*philo)->next, index + 1));
	return (EXIT_SUCCESS);
}

int		link_forks(t_philo *philo, pthread_mutex_t *right_fork, int index)
{
	t_philo	*last;

	last = philo;	
	while (index == 1 && last && last->next)
		last = last->next;
	if (philo->args->number_of_philosophers == 1)
	{
		if (!(philo->right_fork = malloc(sizeof(pthread_mutex_t))))
			return (EXIT_FAILURE);
		pthread_mutex_init(philo->right_fork, NULL);
	}
	else if (index == 1)
		philo->right_fork = last->left_fork;
	else
		philo->right_fork = right_fork;
	if (philo->next)
		return (link_forks(philo->next, philo->left_fork, index + 1));	
	return (EXIT_SUCCESS);
}

int		main(int argc, char **argv)
{
	t_args			*args;
	t_philo			*philo;

	args = NULL;
	philo = NULL;
	write(1, "\n\nSTARTING SIMULATION\n\n\n", 25);
	if (!(args = malloc(sizeof(t_args)))
		|| !(philo = malloc(sizeof(t_philo)))
		|| parse_args(args, argc, argv)
		|| recruit_philosophers(args, &philo, 1)
		|| link_forks(philo, NULL, 1)
		|| start_simulation(philo))
		return (free_philosophers(&args, &philo, EXIT_FAILURE));
//	usleep(2000000);
	
	wait_till_the_end(philo);
	return (free_philosophers(&args, &philo, EXIT_SUCCESS));
}
