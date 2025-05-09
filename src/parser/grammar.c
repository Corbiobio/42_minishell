/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 09:25:45 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/09 15:09:12 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	remove_redirection(t_tokenized_line *input, t_tokenized_line *output)
{
	size_t	i;

	i = 0;
	*output = (t_tokenized_line){.line = input->line};
	while (i < input->nb_token)
	{
		if (is_type_redirect(input->tokens[i]))
		{
			i++;
			if (is_word(input->tokens[i]) && i < input->nb_token)
				i++;
		}
		else if (i < input->nb_token)
		{
			add_token(output, input->tokens[i]);
			i++;
		}
	}
}

int	alloc_cmd_list(t_tokenized_line *line, t_cmd_list *cmd_list)
{
	size_t	i;
	int		cmd_index;
	size_t	counter;

	i = 0;
	cmd_index = 0;
	counter = 1;
	while (i < line->nb_token)
	{
		if (is_word(line->tokens[i]))
			counter++;
		if (line->tokens[i].type == TYPE_PIPE)
		{
			cmd_list->cmds[cmd_index].cmd = ft_calloc(counter, sizeof(int *));
			if (cmd_list->cmds[cmd_index].cmd == 0)
				return (1);
			counter = 1;
			cmd_index++;
		}
		i++;
	}
	cmd_list->cmds[cmd_index].cmd = ft_calloc(counter, sizeof(int *));
	if (cmd_list->cmds[cmd_index].cmd == 0)
		return (1);
	return (0);
}

int	check_empty_cmd(t_cmd_list *cmd_list, t_hash_table *env)
{
	size_t	i;
	t_cmd	cmd;

	if (cmd_list->nb_cmd == 1)
		return (0);
	i = 0;
	while (i < cmd_list->nb_cmd)
	{
		cmd = cmd_list->cmds[i];
		if (cmd.io[0] == -2 && cmd.io[1] == -2)
		{
			if (cmd.cmd[0] == NULL)
				return (print_error_set_status(ERROR_NO_CMD, env));
		}
		i++;
	}
	return (0);
}

int	convert_to_cmd_list(t_tokenized_line *line, t_cmd_list *cmd_list)
{
	size_t	i;
	int		cmd_index;
	int		counter;

	i = -1;
	cmd_index = 0;
	counter = 0;
	while (++i < line->nb_token)
	{
		if (is_word(line->tokens[i]))
		{
			cmd_list->cmds[cmd_index].cmd[counter] = alloc_word(line, i);
			if (cmd_list->cmds[cmd_index].cmd[counter++] == 0)
				return (1);
		}
		if (line->tokens[i].type == TYPE_PIPE)
		{
			cmd_list->cmds[cmd_index].nb_arg = counter - 1;
			cmd_list->cmds[cmd_index++].cmd[counter] = 0;
			counter = 0;
		}
	}
	cmd_list->cmds[cmd_index].nb_arg = counter - 1;
	cmd_list->cmds[cmd_index].cmd[counter] = 0;
	return (0);
}

int	grammarify(t_tokenized_line *line, t_cmd_list *cmd_list, t_hash_table *env)
{
	t_tokenized_line	*line_no_redirect;

	line_no_redirect = malloc(sizeof(t_tokenized_line)
			+ sizeof(t_token) * line->nb_token);
	if (line_no_redirect == 0)
	{
		close_all_fd(cmd_list);
		free_cmd_list(cmd_list);
		return (1);
	}
	remove_redirection(line, line_no_redirect);
	if (alloc_cmd_list(line_no_redirect, cmd_list) == 1
		|| convert_to_cmd_list(line, cmd_list) == 1
		|| check_empty_cmd(cmd_list, env) == 1)
	{
		close_all_fd(cmd_list);
		free_cmd_list(cmd_list);
		return (free_1_return_1(line_no_redirect));
	}
	free(line_no_redirect);
	return (0);
}
