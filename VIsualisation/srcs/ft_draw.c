/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhaenys <rrhaenys@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 03:08:31 by rrhaenys          #+#    #+#             */
/*   Updated: 2019/03/26 17:03:43 by rrhaenys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void
	line_fast(t_data *env, double *p1, double *p2, int color)
{
	int i;
	int x[2];
	int p[2];

	x[0] = abs((int)(p2[Y_P] - p1[Y_P])) >> 1;
	x[1] = abs((int)(p2[X_P] - p1[X_P])) >> 1;
	ft_draw_px(env, (p[0] = p1[X_P]),
				(p[1] = p1[Y_P]), color);
	if (((i = -1) == -1) && abs((int)(p2[X_P] - p1[X_P])) >=
		abs((int)(p2[Y_P] - p1[Y_P])))
		while (++i < abs((int)(p2[X_P] - p1[X_P])))
		{
			BLOCK_2(x[1], abs((int)(p2[X_P] - p1[X_P])), p[1],
			SGN(p2[Y_P] - p1[Y_P]), abs((int)(p2[Y_P] - p1[Y_P])));
			ft_draw_px(env, (p[0] += SGN(p2[X_P] - p1[X_P])), p[1], color);
		}
	else
		while (++i < abs((int)(p2[Y_P] - p1[Y_P])))
		{
			BLOCK_2(x[0], abs((int)(p2[Y_P] - p1[Y_P])), p[0],
			SGN(p2[X_P] - p1[X_P]), abs((int)(p2[X_P] - p1[X_P])));
			ft_draw_px(env, p[0], (p[1] += SGN(p2[Y_P] - p1[Y_P])), color);
		}
}

void
	ft_draw_px(t_data *data, int x, int y, int color)
{
	data->img->data[y * WIN_W + x] = color;
}

void
	line_vertical(t_data *data, int x, int *y, int color)
{
	int	ind_y;

	ind_y = y[0] - 1;
	while (++ind_y <= y[1])
		data->img->data[ind_y * WIN_H + x] = color;
}

void
	ft_linefast_int(t_data *data, int *p1, int *p2, int color)
{
	double	f1[2];
	double	f2[2];

	f1[0] = p1[0];
	f1[1] = p1[1];
	f2[0] = p2[0];
	f2[1] = p2[1];
	line_fast(data, f1, f2, color);
}

void
	draw_cube_size(t_data *data, int x, int y, int color, double size)
{
	int		pos[2];
	int		colors[5];

	colors[0] = 0xc0c0c0;
	colors[1] = 0x0000ff;
	colors[2] = 0x9932cc;
	colors[3] = 0xff0000;
	colors[4] = 0x00ff00;
	pos[0] = 20 + x * 20;
	pos[1] = 20 + y * 20;
	while (size > 1)
		size -= 1.0;
	ft_draw_square(data, pos, 10, 0x008000);
	ft_draw_square(data, pos, 9 * size, colors[color]);
}

void
	draw_cube(t_data *data, int x, int y, int color)
{
	draw_cube_size(data, x, y, color, 1);
}

void
	draw_cube_active(t_data *data, int x, int y, int color)
{
	int		pos[2];
	int		colors[5];

	colors[0] = 0xc0c0c0;
	colors[1] = 0x0000ff;
	colors[2] = 0x9932cc;
	colors[3] = 0xff0000;
	colors[4] = 0x00ff00;
	pos[0] = 20 + x * 20;
	pos[1] = 20 + y * 20;
	ft_draw_square(data, pos, 10, 0xffff00);
	ft_draw_square(data, pos, 8, colors[color]);
}

void
	ft_update_my_arr(t_data *data)
{
	int			magic;
	int			i;
	char		*str;

	if (read(0, &magic, sizeof(int)) <= 0)
		return ;
	++data->mydata->cycles;
	if (magic != VIS_MAGIC)
		exit(0);
	if (data->mydata->param != NULL)
		free(data->mydata->param);
	data->mydata->param = (t_param *)malloc(sizeof(t_param));
	read(0, data->mydata->param, sizeof(t_param));
	i = -1;
	while (++i < MEM_SIZE)
	{
		free(data->mydata->arr[i].str);
		data->mydata->arr[i].str = ft_rebase(data->mydata->param->map[i], 16);
		if (ft_strlen(data->mydata->arr[i].str) == 1)
		{
			str = ft_strdup("00");
			str[1] = data->mydata->arr[i].str[0];
			free(data->mydata->arr[i].str);
			data->mydata->arr[i].str = str;
		}
		data->mydata->arr[i].color = data->mydata->param->map_color[i];
	}
	if (data->mydata->process != NULL)
		free(data->mydata->process);
	data->mydata->process = (t_process *)malloc(sizeof(t_process) *
	(data->mydata->param->proc_nbr));
	data->mydata->process_count = data->mydata->param->proc_nbr;
	i = -1;
	while (++i < data->mydata->param->proc_nbr)
		read(0, &(data->mydata->process[i]), sizeof(t_process));
}

void
	ft_out_params(t_data *data, t_win_par par)
{
	int		width;
	char	*num;

	mlx_string_put(data->mlx_ptr, data->mlx_win, par.x, par.y,
	par.color_str, par.str);
	width = ft_strlen(par.str);
	num = ft_itoa(par.par);
	mlx_string_put(data->mlx_ptr, data->mlx_win, par.x + width * 10 + 5, par.y,
	par.color_num, num);
	free(num);
}

static long long
	ft_get_value(unsigned char *ptr, int size)
{
	long long	res;
	int			i;

	res = 0;
	i = -1;
	while (++i < size)
		res = (res << (8 * i)) + ptr[i];
	return (res);
}

int
	ft_get_reg(t_process *proc, int reg_num)
{
	int		reg;

	reg = ft_get_value(proc->r[reg_num], REG_SIZE);
	return (reg);
}

void
	ft_print_proces(t_data *data, int x, int y, t_process *proc)
{
	char	*str;
	int		reg;
	int		index;

	if (proc == NULL)
		return ;
	ft_out_params(data, (t_win_par){x, y, 0xffffff, 0xffffff, "ID:", proc->id});
	ft_out_params(data, (t_win_par){x, y + 15, 0xffffff, 0xffffff, "Carry:", proc->carry});
	ft_out_params(data, (t_win_par){x + 55, y + 38, 0xffffff, 0xffffff, "Wait:", proc->wait});
	ft_out_params(data, (t_win_par){x + 55, y + 53, 0xffffff, 0xffffff, "Pos_y:", (proc->pc - proc->map) / 64});
	ft_out_params(data, (t_win_par){x + 55, y + 68, 0xffffff, 0xffffff, "Pos_x:", (proc->pc - proc->map) % 64});
	ft_out_params(data, (t_win_par){x + 55, y + 83, 0xffffff, 0xffffff, "Livin:", data->mydata->cycles - proc->livin});
	ft_out_params(data, (t_win_par){x + 55, y + 98, 0xffffff, 0xffffff, "Op.id:", proc->op.id});
	mlx_string_put(data->mlx_ptr, data->mlx_win, x, y + 38, 0xffffff, "Reg:");
	index = -1;
	while (++index < REG_NUMBER)
	{
		reg = ft_get_reg(proc, index);
		str = ft_itoa(reg);
		mlx_string_put(data->mlx_ptr, data->mlx_win,
		x, y + 55 + 15 * (index), 0xffffff, str);
		free(str);
	}
}

t_process
	*ft_get_process_id(t_data *data, int id)
{
	if (id < data->mydata->process_count)
		return (&data->mydata->process[data->mydata->process_count - 1 - id]);
	return (NULL);
}

void
	ft_print_champs(t_data *data, int x, int y, t_champ *champs)
{
	mlx_string_put(data->mlx_ptr, data->mlx_win, x, y, 0xffffff, "Name:");
	mlx_string_put(data->mlx_ptr, data->mlx_win, x + 80, y, 0x00ff00, champs->name);
	mlx_string_put(data->mlx_ptr, data->mlx_win, x, y + 15, 0xffffff, "Comment:");
	mlx_string_put(data->mlx_ptr, data->mlx_win, x + 80, y + 15, 0x00ff00, champs->comment);
	ft_out_params(data, (t_win_par){x, y + 30, 0xffffff, 0x00ff00, "champ_size:", champs->champ_size});
	ft_out_params(data, (t_win_par){x, y + 45, 0xffffff, 0x00ff00, "color:", champs->color});
	ft_out_params(data, (t_win_par){x, y + 60, 0xffffff, 0x00ff00, "n:", champs->n});
	ft_out_params(data, (t_win_par){x, y + 75, 0xffffff, 0x00ff00, "magic:", champs->magic});
}

int
	ft_draw(t_data *data)
{
	int		index;
	int		size;
	int		pos;
	int		delta;

	mlx_put_image_to_window(data->mlx_ptr, data->mlx_win,
		data->img->img_ptr, 0, 0);
	if (data->mydata->run)
	{
		ft_update_my_arr(data);
		mlx_string_put(data->mlx_ptr, data->mlx_win,
		WIN_W - 495, 20, 0x00ff00, "< RUN >");
	}
	else
		mlx_string_put(data->mlx_ptr, data->mlx_win,
		WIN_W - 495, 20, 0xff0000, "< STOP >");	
	size = 64;
	index = data->mydata->first_proces - 1;
	mlx_string_put(data->mlx_ptr, data->mlx_win, WIN_W - 700, WIN_H - 300, 0xffffff, "Process");
	ft_out_params(data, (t_win_par){WIN_W - 700, WIN_H - 285, 0xffffff, 0xffffff, "from:", (data->mydata->first_proces + 1)});
	ft_out_params(data, (t_win_par){WIN_W - 700, WIN_H - 270, 0xffffff, 0xffffff, "count:", data->mydata->process_count});
	delta = 0;
	while (++index < data->mydata->process_count && index < data->mydata->first_proces + 10)
	{
		while (ft_get_process_id(data, index + delta) == NULL)
			++delta;
		ft_print_proces(data, 13 + 175 * (index - data->mydata->first_proces), WIN_H - 310, ft_get_process_id(data, index + delta));
	}
	index = -1;
	while (++index < data->mydata->process_count)
	{
		pos = data->mydata->process[index].pc - data->mydata->process[index].map;
		mlx_string_put(data->mlx_ptr, data->mlx_win,
		13 + 30 * (pos % size), 10 + 15 * (pos / size), 0xffffff, "__");
	}
	index = -1;
	while (++index < MEM_SIZE)
	{
		mlx_string_put(data->mlx_ptr, data->mlx_win,
		13 + 30 * (index % size), 10 + 15 * (index / size),
		data->mydata->color[data->mydata->arr[index].color],
		data->mydata->arr[index].str);
	}
	ft_out_params(data, (t_win_par){WIN_W - 500, 50, 0xffffff, 0xffffff, "Cycles:", data->mydata->cycles});
	ft_out_params(data, (t_win_par){WIN_W - 500, 70, 0xffffff, 0xffffff, "Players:", data->mydata->param->players});
	ft_out_params(data, (t_win_par){WIN_W - 500, 90, 0xffffff, 0xffffff, "cycles_to_die:", data->mydata->param->cycles_to_die});
	ft_out_params(data, (t_win_par){WIN_W - 500, 110, 0xffffff, 0xffffff, "current_cycle:", data->mydata->param->current_cycle});
	ft_out_params(data, (t_win_par){WIN_W - 500, 130, 0xffffff, 0xffffff, "last_check:", data->mydata->param->last_check});
	ft_out_params(data, (t_win_par){WIN_W - 500, 150, 0xffffff, 0xffffff, "checks:", data->mydata->param->checks});
	ft_out_params(data, (t_win_par){WIN_W - 500, 170, 0xffffff, 0xffffff, "live_nbr:", data->mydata->param->live_nbr});
	ft_out_params(data, (t_win_par){WIN_W - 500, 190, 0xffffff, 0xffffff, "winner:", data->mydata->param->winner});
	index = -1;
	while (++index < data->mydata->param->players)
		ft_print_champs(data, WIN_W - 550, 250 + 100 * (index), &(data->mydata->param->champs[index]));
	return (1);
}