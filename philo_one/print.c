/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 19:22:20 by user42            #+#    #+#             */
/*   Updated: 2021/02/16 01:41:15 by amartin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_one.h"

/*
** get_time: returns time in ms (milliseconds)
*/

size_t		get_time(void)
{
	struct timeval	start;

	if (gettimeofday(&start, NULL) == -1)
		return (get_time());
	return (start.tv_sec * 1000 + start.tv_usec / 1000);
}

static int	len_of_timestamp_x(size_t time, int index)
{
	int		len;

	len = 0;
	if (!index)
		len -= 2;
	if (!time)
		len++;
	while (time)
	{
		len++;
		time /= 10;
	}
	while (index)
	{
		len++;
		index /= 10;
	}
	return (len);
}

/*
** print_activity: prints the change of state of a philosopher
*/

void		print_activity(size_t time, int index, char *activity,
				pthread_mutex_t *lock)
{
	char	msg[30];
	int		len;
	int		i;
	int		print_lock;

	if ((print_lock = activity[1]) != 'd')
		print_lock = 0;
	memset(msg, ' ', sizeof(msg));
	msg[29] = '\n';
	len = len_of_timestamp_x(time, index);
	i = len + 1;
	while (len >= 0 && index && (msg[len--] = index % 10 + '0'))
		index /= 10;
	msg[len--] = ' ';
	msg[len] = 0 + '0';
	while (len >= 0 && (time) && (msg[len--] = (time) % 10 + '0'))
		time /= 10;
	while (activity && *activity && i < 29)
		msg[i++] = *activity++;
	pthread_mutex_lock(lock);
	write(1, msg, 30);
	if (!print_lock)
		pthread_mutex_unlock(lock);
}
