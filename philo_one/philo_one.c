/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 15:04:16 by user42            #+#    #+#             */
/*   Updated: 2020/12/01 20:55:08 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_one.h"

void		*random_function(void *arg)
{
	(void)arg;
	printf("\nI'm a beautiful thread");
	return (NULL);
}

pthread_t	*create_philosophers(int nb_of_philosophers)
{
	pthread_t	*philosophers;
	int			i;

	philosophers = (pthread_t*)malloc(sizeof(pthread_t) * nb_of_philosophers);
	if (!philosophers)
		return (NULL);
	i = 0;
	while (i < nb_of_philosophers)
	{
		printf("\nPhilosopher %d! Begginning", i + 1);
		pthread_create(&philosophers[i], NULL, random_function, NULL);
		i++;
	}
	return (philosophers);
}

int		main(int argc, char **argv)
{
	pthread_t	*philosophers;
	int			i;
	t_args		arg;

	(void)argc;
	if (init_args(&arg, argv) < 0)
		return (0);
	philosophers = create_philosophers(arg.number_of_philosophers);
	i = 0;
	while (philosophers && i < (arg.number_of_philosophers))
	{
		pthread_join(philosophers[i], NULL);
		printf("\nPhilosopher %d! end", i + 1);
		i++;
	}
	if (philosophers)
		free(philosophers);
	return (0);
}
