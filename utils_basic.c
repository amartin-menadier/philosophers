/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_basic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 18:08:56 by user42            #+#    #+#             */
/*   Updated: 2020/12/01 20:43:32 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosophers.h"

int		ft_basic_strlen(char *str)
{
	int	len;

	len = 0;
	while (str && str[len])
		len++;
	return (len);
}

void	ft_basic_putstr(char *stra, char *strb, char *strc, char *strd)
{
	if (stra && stra[0])
	{
		write(1, stra, ft_basic_strlen(stra));
		write(1, " ", 1);
	}
	if (strb  && strb[0])
	{
		write(1, strb, ft_basic_strlen(strb));
		write(1, " ", 1);
	}
	if (strc  && strc[0])
	{
		write(1, strc, ft_basic_strlen(strc));
		write(1, " ", 1);
	}
	if (strd && strd[0])
		write(1, strd, ft_basic_strlen(strd));
}

int		ft_basic_atoi(char *str)
{
	int		i;
	int		n;

	if (!str)
	{
		write(1, "\nError: missing arg", 13);
		return (-1);
	}
	i = 0;
	n = 0;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		n *= 10;
		n += (str[i] - '0');
		i++;
	}
	if (str[i])
	{
		write(1, "\nError: args shall only have digit characters", 46);
		return (-1);
	}
	return (n);
}
