/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amerlon- <amerlon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 22:11:05 by amerlon-          #+#    #+#             */
/*   Updated: 2019/01/27 09:12:21 by amerlon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_putendl(char const *s)
{
	int	i;

	if (!s)
		return (-1);
	i = -1;
	while (s[++i])
		write(1, s + i, 1);
	write(1, "\n", 1);
	return (i + 1);
}