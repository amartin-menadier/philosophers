/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_time_and_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:05:35 by user42            #+#    #+#             */
/*   Updated: 2020/12/07 18:46:40 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosophers.h"

/*
** get_time: returns time in µs (microseconds)
** Multiply the return value by 1000 to get ms (milliseconds)
*/

size_t	get_time(void)
{
	struct timeval	start;

	if (gettimeofday(&start, NULL) == -1)
		return (get_time());
	return ((start.tv_sec * 1000000) + start.tv_usec);
}

int		basic_strlen(char *str)
{
	int	len;

	len = 0;
	while (str && str[len])
		len++;
	return (len);
}

/*
** basic_itoa: only for positive values
*/

char	*basic_itoa(size_t n)
{
	size_t		copy;
	int			len;
	char		*fresh;

	copy = n;
	len = 0;
	while (copy != 0)
	{
		copy /= 10;
		len++;
	}
	if (len == 0)
		len = 1;
	if (!(fresh = (char *)malloc((len + 1) * sizeof(char))))
		return (NULL);
	fresh[len] = '\0';
	len--;
	while (len >= 0)
	{
		fresh[len] = (n % 10) + '0';
		len--;
		n /= 10;
	}
	return (fresh);
}

char	*get_msg(char *time, char *index, char *activity)
{
	char	*msg;
	int		len;
	int		i;

	len = basic_strlen(time) + basic_strlen(index) + basic_strlen(activity) + 3;
	if (!index)
		len--;
	if (!(msg = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	msg[len] = '\0';
	i = 0;
	while (i < len && time && *time)
		msg[i++] = *time++;
	msg[i++] = ' ';
	while (i < len && index && *index)
		msg[i++] = *index++;
	if (index)
		msg[i++] = ' ';
	while (i < len && activity && *activity)
		msg[i++] = *activity++;
	msg[i] = '\n';
	return (msg);
}

/*
** print_activity: prints the change of state of a philosopher
*/

void	print_activity(size_t time, int index, char *activity)
{
	char	*msg;
	char	*timestamp;
	char	*index_str;

	time /= 1000;
	timestamp = basic_itoa(time);
	if (index)
		index_str = basic_itoa(index);
	else
		index_str = NULL;
	msg = get_msg(timestamp, index_str, activity);
	write(1, msg, basic_strlen(msg));
	free(timestamp);
	if (index_str)
		free(index_str);
	free(msg);
}
