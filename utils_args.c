/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 19:22:20 by user42            #+#    #+#             */
/*   Updated: 2020/12/01 20:43:36 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosophers.h"

int		init_args(t_args *arg, char **argv)
{
	if (!argv || !argv[1] || !argv[2] || !argv[3] || !argv[4])
	{
		write(1, "\nError: At least 4 args are needed", 35);
		return (-1);
	}
	if ((arg->number_of_philosophers = ft_basic_atoi(argv[1])) < 0
		|| (arg->time_to_die = ft_basic_atoi(argv[2])) < 0
		|| (arg->time_to_eat = ft_basic_atoi(argv[3])) < 0
		|| (arg->time_to_sleep = ft_basic_atoi(argv[4])) < 0)
			return (-1);
	if (argv[5])
	{
		arg->number_of_time_each_philosopher_must_eat = ft_basic_atoi(argv[5]);
		if (arg->number_of_time_each_philosopher_must_eat < 0)
			return (-1);
	}
	else
		arg->number_of_time_each_philosopher_must_eat = -1;
	return (0);
}
