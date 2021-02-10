/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amartin- <amartin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:05:35 by user42            #+#    #+#             */
/*   Updated: 2021/02/11 00:51:58 by amartin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_three.h"

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

	len = 1;
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

void		print_activity(size_t time, int index, char *activity, sem_t **lock)
{
	char	msg[100];
	int		len;
	int		activity_index;

	if (lock && *lock)
		sem_wait(*lock);
	else
		return ;
	if (activity[1] == 'd' && lock && !sem_close(*lock))
		*lock = NULL;
	memset(msg, '\0', sizeof(msg));
	len = len_of_timestamp_x(time, index);
	activity_index = len + 1;
	while (len >= 0 && index && (msg[len--] = index % 10 + '0'))
		index /= 10;
	msg[len--] = ' ';
	msg[len] = 0 + '0';
	while (len >= 0 && (time) && (msg[len--] = (time) % 10 + '0'))
		time /= 10;
	while (activity && *activity && activity_index < 29)
		msg[activity_index++] = *activity++;
	msg[activity_index] = '\n';
	write(1, msg, 100);
	if (lock && *lock)
		sem_post(*lock);
}
