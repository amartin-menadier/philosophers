/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:53:41 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 16:11:32 by amartin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_two.h"

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

static char	*basic_itoa(int nb)
{
	char	*str;
	int		len;
	int		tmp;

	tmp = nb;
	len = 0;
	while (tmp)
	{
		tmp /= 10;
		len++;
	}
	if (!nb)
		len = 1;
	str = (char *)malloc(sizeof(char) * len + 1);
	str[len--] = '\0';
	while (len >= 0)
	{
		str[len--] = nb % 10 + 48;
		nb /= 10;
	}
	return (str);
}

static int	philo_alone(int time_to_die)
{
	int		len;
	char	*time_of_death;

	if (!(time_of_death = basic_itoa(time_to_die)))
		return (EXIT_FAILURE);
	write(1, "0 1 is thinking\n", 17);
	write(1, "0 1 has taken a fork\n", 22);
	usleep(time_to_die * 1000);
	len = 0;
	while (time_of_death[len])
		len++;
	write(1, time_of_death, len);
	write(1, " 1 died\n", 9);
	free(time_of_death);
	return (EXIT_FAILURE);
}

static int	arg_error(int my_errno)
{
	if (my_errno == ARG_COUNT)
		write(2, "\nError: Invalid number of arguments", 36);
	else if (my_errno == ARG_FORMAT)
		write(2, "\nError: Invalid format of an argument", 38);
	return (EXIT_FAILURE);
}

/*
** parse_args: Checks the nb of args, if they are digits-only and 'atoies' them
*/

int			parse_args(t_args *arg, int argc, char **argv)
{
	arg->start_time = 0;
	if (!argv[5])
		arg->times_must_eat = -1;
	if (argc < 5 || argc > 6)
		return (arg_error(ARG_COUNT));
	else if ((arg->number_of_philosophers = basic_atoi(argv[1])) <= 0
		|| (arg->time_to_die = basic_atoi(argv[2])) < 0
		|| (arg->time_to_eat = basic_atoi(argv[3])) < 0
		|| (arg->time_to_sleep = basic_atoi(argv[4])) < 0
		|| (argv[5] && (arg->times_must_eat = (int)basic_atoi(argv[5])) < 0))
		return (arg_error(ARG_FORMAT));
	if (arg->number_of_philosophers == 1)
		return (philo_alone(arg->time_to_die));
	return (EXIT_SUCCESS);
}
