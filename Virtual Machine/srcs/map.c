/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorwin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 17:08:46 by jcorwin           #+#    #+#             */
/*   Updated: 2019/03/14 17:50:54 by jcorwin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void		map_init(t_param *p)
{
	int		i;
	int		step;

	i = -1;
	while (++i <= MEM_SIZE)
		p->map[i] = 0;
	step = MEM_SIZE / p->players;
	i = -1;
	while (++i < p->players)
		ft_memcpy(&p->map[i * step], p->champs[i].champ,
											p->champs[i].champ_size);
}

void		map_print(t_param *p)
{
	int		line;

	line = 0;
	while (line < MEM_SIZE)
	{
		print_bytes(&p->map[line], 64);
		line += 64;
	}
}

void		print_bytes(unsigned char *str, int len)
{
	if (len == 0)
		len = ft_strlen((char *)str);
	while (len--)
	{
		if (*str < 16)
			ft_printf("0");
		ft_printf("%hhx ", *str++);
	}
	ft_printf("\n");
}