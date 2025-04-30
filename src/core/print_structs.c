/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_structs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:02:17 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/30 12:11:46 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdio.h>

char	*type_to_str(t_type type)
{
	if (type == TYPE_WORD)
		return ("word");
	if (type == TYPE_DEAD_TOKEN)
		return ("dead token");
	if (type == TYPE_WHITESPACE)
		return ("whitespace");
	if (type == TYPE_SINGLE_QUOTE)
		return ("single_quote");
	if (type == TYPE_DOUBLE_QUOTE)
		return ("double_quote");
	if (type == TYPE_GREATER)
		return ("greater");
	if (type == TYPE_LESSER)
		return ("lesser");
	if (type == TYPE_GREATER_GREATER)
		return ("greater_greater");
	if (type == TYPE_LESSER_LESSER)
		return ("lesser_lesser");
	if (type == TYPE_PIPE)
		return ("pipe");
	if (type == TYPE_DOLLAR)
		return ("dollar");
	if (type == TYPE_WORD_QUOTED)
		return ("quoted word");
	return ("error");
}

void	print_tokens(t_tokenized_line *line)
{
	size_t	i;

	printf("line: %s\n nb_tokens: %zu\n", line->line, line->nb_token);
	i = 0;
	while (i < line->nb_token)
	{
		printf("[ %.*s ] type: %s\n", (int)line->tokens[i].len,
			line->line + line->tokens[i].pos,
			type_to_str(line->tokens[i].type));
		i++;
	}
}

void	print_cmds(t_cmd_list *list)
{
	size_t	i;
	int		j;

	printf("nb_cmd: %zu\n", list->nb_cmd);
	i = 0;
	while (i < list->nb_cmd)
	{
		printf("i: %i, o: %i\n", list->cmds[i].io[0], list->cmds[i].io[1]);
		printf("nb_arg: %zu\n", list->cmds[i].nb_arg);
		j = 0;
		while (list->cmds[i].cmd[j] != 0)
		{
			printf("%s, ", list->cmds[i].cmd[j]);
			j++;
		}
		printf("\n");
		i++;
	}
}
