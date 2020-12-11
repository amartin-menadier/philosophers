/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_time_and_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:05:35 by user42            #+#    #+#             */
/*   Updated: 2020/12/11 13:50:17 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo_three.h"

/*
** get_time: returns time in µs (microseconds)
** Multiply the return value by 1000 to get ms (milliseconds)
*/

size_t	get_time(void)
{
	struct timeval	start;

	if (gettimeofday(&start, NULL) == -1)
		return (get_time());
	return (start.tv_sec * 1000 + start.tv_usec / 1000);
}

int		len_of_timestamp_x(size_t time, int index)
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

void	print_activity(size_t time, int index, char *activity, sem_t *lock)
{
	char	msg[100];
	int		len;
	int		activity_index;
	int		unlock;

	memset(msg, '\0', sizeof(msg));
	unlock = 0;
	if (activity[1] != 'd' && ((time = time - time % 10) >= 0))
		unlock = 1;
	len = len_of_timestamp_x(time, index);
	activity_index = len + 1;
	while (len >= 0 && index && (msg[len--] = index % 10 + '0'))
		index /= 10;
	msg[len--] = ' ';
	msg[len] = 0 + '0';
	while (len >= 0 && (time) && (msg[len--] = (time) % 10 + '0'))
		time /= 10;
	while (activity && *activity && activity_index < 99)
		msg[activity_index++] = *activity++;
	msg[activity_index] = '\n';
	sem_wait(lock);
	write(1, msg, 100);
	if (unlock)
		sem_post(lock);
}
