/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorwin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 21:48:39 by jcorwin           #+#    #+#             */
/*   Updated: 2019/03/18 19:57:36 by jcorwin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
# define VM_H

# include "op.h"
# include "libft.h"
# include "ft_printf.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>

# define DEFAULT_MODE 0
# define DUMP_MODE 1
# define STEP_MODE 2
# define NCURSES_MODE 3
# define VIS_MODE 4

# define LIVE 1
# define LD 2
# define ST 3
# define ADD 4
# define SUB 5
# define AND 6
# define OR 7
# define XOR 8
# define ZJMP 9
# define LDI 10
# define STI 11
# define FORK 12
# define LLD 13
# define LLDI 14
# define LFORK 15
# define AFF 16

/* чемпион */
typedef struct		s_champ
{
	char			*file;
	unsigned int	magic;
	char			name[PROG_NAME_LENGTH + 1];
	char			comment[COMMENT_LENGTH + 1];
	unsigned int	champ_size;
	char			champ[CHAMP_MAX_SIZE + 1];
	char			null_term[4];
	int				color;
}					t_champ;

/*ячейка памяти*/
typedef struct		s_cage
{
	char		value;
	int			color;
	int			process;
}					t_cage;

/*операция*/
typedef struct		s_op
{
	unsigned char	*ptr;
	int				id;
	char			type;
	unsigned char	arg_type[3];
	long long int	arg[3];
}					t_op;

/*каретка*/
typedef struct		s_process
{
	int					id;
	unsigned char		r[REG_NUMBER][REG_SIZE];
	int					carry;
	unsigned char		*map;
	unsigned char		*pc;
	int					wait;
	int					livin;
	int					step;
	int					moved;
	t_op				op;
	struct s_process	*next;
}					t_process;

/*флаги программы*/
typedef	struct		s_flags
{
	char	comment;
	int		step;
	char	verb;
	char	mode;
	int		id;
}					t_flags;

/*основные параметры*/
typedef struct		s_param
{
	int				champ_arg;
	t_champ			champs[MAX_PLAYERS];
	int				players;
	t_flags			flag;
	unsigned char	map[MEM_SIZE + 1];
	t_process		*process;
	int				cycles_to_die;
	unsigned int	current_cycle;
	int				winner;
}					t_param;

typedef struct		s_funs
{
	char		*name;
	int			nb_params;
	char		params_type[3];
	int			id;
	int			cycles;
	char		description[50];
	int			codage;
	int			label_size;
	int			(*f_check)(t_process *p);
	void		(*f_do)(t_param *par, t_process *p);
}					t_funs;

int					get_champ(char *str, t_param *p, int id);

void				usage(void);
void				help(void);
void				malloc_err(void);

void				print_bytes(unsigned char *str, int len);
void				map_init(t_param *p);
void				map_print(t_param *p);

void				op_read(t_process *p);
void				op_args(t_process *p);

void				process_new(t_param *p, t_process *parent, unsigned char *pc);
void				process_die(t_param *p, t_process *die);

unsigned char		*get_step(unsigned char *map, unsigned char *ptr, int step);
long long			get_value(unsigned char *map, unsigned char *ptr, int size);
void				set_value(unsigned char *map, unsigned char *dst,
												long long src, int size);

void				do_op(t_param *param, t_process *process);
void				start_game(t_param *param);

void				process_print(t_process *p);

void				op_live(t_param *param, t_process *process);
void				op_ld(t_param *param, t_process *process);
void				op_lld(t_param *param, t_process *process);
void				op_st(t_param *param, t_process *process);
void				op_add(t_param *param, t_process *process);
void				op_sub(t_param *param, t_process *process);
void				op_and(t_param *param, t_process *process);
void				op_or(t_param *param, t_process *process);
void				op_xor(t_param *param, t_process *process);
void				op_zjmp(t_param *param, t_process *process);
void				op_sti(t_param *param, t_process *process);
void				op_ldi(t_param *param, t_process *process);
void				op_lldi(t_param *param, t_process *process);
void				op_fork(t_param *param, t_process *process);
void				op_lfork(t_param *param, t_process *process);
void				op_aff(t_param *param, t_process *process);

int					op_check(t_process *p);
int					op_check_live(t_process *p);
int					op_check_ld(t_process *p);
int					op_check_st(t_process *p);
int					op_check_add(t_process *p);
int					op_check_sub(t_process *p);
int					op_check_and(t_process *p);
int					op_check_or(t_process *p);
int					op_check_xor(t_process *p);
int					op_check_zjmp(t_process *p);
int					op_check_ldi(t_process *p);
int					op_check_sti(t_process *p);
int					op_check_fork(t_process *p);
int					op_check_lld(t_process *p);
int					op_check_lldi(t_process *p);
int					op_check_lfork(t_process *p);
int					op_check_aff(t_process *p);

static t_funs		g_op_tab[17] =
{
	{"live", 1, {T_DIR}, 1, 10, "alive", 0, 4, &op_check_live, &op_live},
	{"ld", 2, {T_DIR | T_IND, T_REG}, 2, 5, "load", 1, 4,
												&op_check_ld, &op_ld},
	{"st", 2, {T_REG, T_IND | T_REG}, 3, 5, "store", 1, 4,
												&op_check_st, &op_st},
	{"add", 3, {T_REG, T_REG, T_REG}, 4, 10, "addition", 1, 4,
												&op_check_add, &op_add},
	{"sub", 3, {T_REG, T_REG, T_REG}, 5, 10, "soustraction", 1, 4,
												&op_check_sub, &op_sub},
	{"and", 3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}, 6, 6,
		"et (and  r1, r2, r3   r1&r2 -> r3", 1, 4, &op_check_and, &op_and},
	{"or", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 7, 6,
		"ou  (or   r1, r2, r3   r1 | r2 -> r3", 1, 4, &op_check_or, &op_or},
	{"xor", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 8, 6,
		"ou (xor  r1, r2, r3   r1^r2 -> r3", 1, 4, &op_check_xor, &op_xor},
	{"zjmp", 1, {T_DIR}, 9, 20, "jump if zero", 0, 2,
												&op_check_zjmp, &op_zjmp},
	{"ldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 10, 25,
		"load index", 1, 2, &op_check_ldi, &op_ldi},
	{"sti", 3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}, 11, 25,
		"store index", 1, 2, &op_check_sti, &op_sti},
	{"fork", 1, {T_DIR}, 12, 800, "fork", 0, 2, &op_check_fork, &op_fork},
	{"lld", 2, {T_DIR | T_IND, T_REG}, 13, 10, "long load", 1, 4,
												&op_check_lld, &op_lld},
	{"lldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 14, 50,
		"long load index", 1, 2, &op_check_lldi, &op_lldi},
	{"lfork", 1, {T_DIR}, 15, 1000, "long fork", 0, 2,
											&op_check_lfork, &op_lfork},
	{"aff", 1, {T_REG}, 16, 2, "aff", 1, 4, &op_check_aff, &op_aff},
	{0, 0, {0}, 0, 1, 0, 0, 0, 0, 0}
};

#endif
