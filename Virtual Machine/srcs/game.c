/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorwin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/17 01:25:13 by jcorwin           #+#    #+#             */
/*   Updated: 2019/03/19 09:57:32 by jcorwin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static void		process_act(t_param *param, t_process *process)
{
	if (process->wait == 0)
	{
		if (process->op.type > 0 && process->op.type < 17)
		{
			op_args(process);
			if (g_op_tab[process->op.id].f_check(process))
				g_op_tab[process->op.id].f_do(param, process);
			process->pc = process->op.ptr;
			process->op.type = 0;
		}
		else 
		{
			process->op.id = 0;
			process->op.type = *process->pc;
			while (process->op.type != g_op_tab[process->op.id].id
					&&process->op.id < 16)
				++process->op.id;
			process->wait = g_op_tab[process->op.id].cycles;
		}
	}
	else if (process->wait == -1)
	{
		process->wait = 1;
		process->pc = get_step(process->map, process->pc, 1);
	}
	--process->wait;
}

static void		check_cycle(t_param *param)
{
	t_process	*pr;

	if (++param->current_cycle >= param->last_check + CYCLE_TO_DIE)
	{
		param->last_check = param->current_cycle;
		pr = param->process;
		while (pr)
		{
			if (pr->livin < param->current_cycle - CYCLE_TO_DIE)
				pr = process_kill(param, pr);
			else
				pr = pr->next;
		}
		if (++param->checks == MAX_CHECKS || param->live_nbr >= NBR_LIVE)
		{
			param->cycles_to_die -= CYCLE_DELTA;
			param->checks = 0;
		}
		param->live_nbr = 0;
	}
}

void			start_game(t_param *param)
{
	t_process	*tmp;
	char		c = 0;

	while (param->process)
	{
		check_cycle(param);
		tmp = param->process;
		while (tmp)
		{
			process_act(param, tmp);
			tmp = tmp->next;
		}
		ft_printf("cycle - %d\n", param->current_cycle);
//		map_print(param);
		if (param->flag.step && !(param->current_cycle % param->flag.step))
			map_print(param);
		if (param->current_cycle == param->flag.dump)
		{
			map_print(param);
			break ;
		}
//		while (c != '\n')
//			read(0, &c, 1);
		c = 0;
	}
}