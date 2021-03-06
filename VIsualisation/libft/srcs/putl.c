/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorwin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 22:49:32 by jcorwin           #+#    #+#             */
/*   Updated: 2019/02/26 01:34:18 by jcorwin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*putl(char *line, char *del)
{
	write(1, line, ft_strlen(line));
	if (del)
		putl(del, NULL);
	return (NULL);
}
