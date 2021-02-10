/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 19:22:20 by user42            #+#    #+#             */
/*   Updated: 2020/12/22 22:17:19 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_one.h"

/*
** basic_atoi: only for positive values
*/

static int	basic_atoi(char *str)
{
	int		i;
	size_t	n;

	i = 0;
	n = 0;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		n *= 10;
		n += (str[i] - '0');
		i++;
	}
	if (str[i] || n > 2147483647)
		return (-1);
	return ((int)n);
}

/*
** parse_args: Checks the nb of args, if they are digits-only and 'atoies' them
*/

int			parse_args(t_args *arg, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		write(2, "\nError: Invalid number of arguments", 36);
		return (EXIT_FAILURE);
	}
	if ((arg->number_of_philosophers = basic_atoi(argv[1])) <= 0
		|| (arg->time_to_die = basic_atoi(argv[2])) < 0
		|| (arg->time_to_eat = basic_atoi(argv[3])) < 0
		|| (arg->time_to_sleep = basic_atoi(argv[4])) < 0
		|| (argv[5]
		&& (arg->times_must_eat = basic_atoi(argv[5])) < 0))
	{
		write(2, "\nError: Invalid format of an argument", 38);
		return (EXIT_FAILURE);
	}
	if (!argv[5])
		arg->times_must_eat = -1;
	arg->start_time = 0;
	arg->print_lock = NULL;
	return (EXIT_SUCCESS);
}