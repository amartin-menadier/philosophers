/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_basic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 18:08:56 by user42            #+#    #+#             */
/*   Updated: 2020/12/04 21:57:58 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosophers.h"

int		basic_strlen(char *str)
{
	int	len;

	len = 0;
	while (str && str[len])
		len++;
	return (len);
}

char	*basic_itoa(size_t n)
{
	size_t		copy;
	int		len;
	char	*fresh;

	copy = n;
	len = 0;
	while (copy != 0)
	{
		copy /= 10;
		len ++;
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
