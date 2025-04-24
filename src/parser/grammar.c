/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 09:25:45 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/24 09:35:39 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

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
			if (input->tokens[i].type == TYPE_WORD && i < input->nb_token)
				i++;
		}
		else if (i < input->nb_token)
		{
			add_token(output, input->tokens[i]);
			i++;
		}
	}
	print_tokens(output);
}

void	alloc_cmd_list(t_tokenized_line *line, t_cmd_list *cmd_list)
{
	size_t	i;
	int		cmd_index;
	size_t	counter;

	i = 0;
	cmd_index = 0;
	counter = 1;
	while (i < line->nb_token)
	{
		if (line->tokens[i].type == TYPE_WORD)
			counter++;
		if (line->tokens[i].type == TYPE_PIPE)
		{
			cmd_list->cmds[cmd_index].cmd = malloc(sizeof(int *) * counter);
			counter = 1;
			cmd_index++;
		}
		i++;
	}
	cmd_list->cmds[cmd_index].cmd = malloc(sizeof(int *) * counter);
}

void	convert_to_cmd_list(t_tokenized_line *line, t_cmd_list *cmd_list)
{
	size_t	i;
	int		cmd_index;
	int		counter;

	i = 0;
	cmd_index = 0;
	counter = 0;
	while (i < line->nb_token)
	{
		if (line->tokens[i].type == TYPE_WORD)
		{
			cmd_list->cmds[cmd_index].cmd[counter] = alloc_word(line, i);
			counter++;
		}
		if (line->tokens[i].type == TYPE_PIPE)
		{
			cmd_list->cmds[cmd_index].cmd[counter] = 0;
			counter = 0;
			cmd_index++;
		}
		i++;
	}
	cmd_list->cmds[cmd_index].cmd[counter] = 0;
	print_cmds(cmd_list);
}

void	grammarify(t_tokenized_line *line, t_cmd_list *cmd_list)
{
	t_tokenized_line	*line_no_redirect;

	line_no_redirect = malloc(sizeof(t_tokenized_line) + sizeof(t_token) * line->nb_token);
	if (line_no_redirect == 0)
		return ;
	remove_redirection(line, line_no_redirect);
	alloc_cmd_list(line_no_redirect, cmd_list);
	convert_to_cmd_list(line_no_redirect, cmd_list);
	free(line_no_redirect);
}
