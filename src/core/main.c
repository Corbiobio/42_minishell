/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:04:17 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/09 15:12:33 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "parser.h"
#include <readline/readline.h>
#include <readline/history.h>

volatile sig_atomic_t	g_signum = 0;

void	free_cmd_list(t_cmd_list *list)
{
	size_t	i;

	i = 0;
	while (i < list->nb_cmd)
	{
		if (list->cmds[i].cmd)
			ft_free_split(list->cmds[i].cmd);
		i++;
	}
	free(list);
}

void	command_loop(t_hash_table *env, struct termios old_termios)
{
	char			*line;
	t_cmd_list		*cmd_list;

	while (42)
	{
		set_signal_handler_main(old_termios);
		line = readline("beurre_demishell$ ");
		if (g_signum == SIGINT)
			reset_signum_save_status(env);
		if (!line)
			return ;
		if (*line)
			add_history(line);
		cmd_list = parser(line, env);
		if (!cmd_list)
			continue ;
		if (exec_cdms_list(cmd_list, env, old_termios) == 42)
		{
			free_cmd_list(cmd_list);
			return ;
		}
		free_cmd_list(cmd_list);
	}
}

int	get_termios(t_hash_table *env_table, struct termios *old_termios)
{
	if (isatty(STDIN_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, old_termios) == -1)
		{
			table_delete_table(env_table);
			return (1);
		}
	}
	else
		ft_memset(old_termios, 0, sizeof(struct termios));
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_hash_table	*env_table;
	struct termios	old_termios;
	int				status;

	status = 1;
	if (ac > 1)
		return (EXIT_FAILURE);
	env_table = convert_env_to_table(env);
	if (env_table == 0)
		return (EXIT_FAILURE);
	if (get_termios(env_table, &old_termios) == 1)
		return (EXIT_FAILURE);
	command_loop(env_table, old_termios);
	rl_clear_history();
	safe_atoi(table_search(env_table, "?"), &status);
	table_delete_table(env_table);
	tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
	write(STDERR_FILENO, "exit\n", 5);
	return (status);
	(void)av;
}
