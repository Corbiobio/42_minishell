/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_conditions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:49:12 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/01 18:18:08 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	file_opening_did_not_fail(t_cmd cmd)
{
	if (cmd.io[0] == -1 || cmd.io[1] == -1)
		return (0);
	return (1);
}

int	next_token_is_word(t_tokenized_line *line, size_t token_index)
{
	if (token_index + 1 < line->nb_token)
	{
		if (is_word(line->tokens[token_index + 1]))
			return (1);
	}
	return (0);
}

int	close_all_fd(t_cmd_list *cmd_list)
{
	size_t	i;

	i = 0;
	while (i < cmd_list->nb_cmd)
	{
		if (cmd_list->cmds[i].io[0] >= 0)
			close(cmd_list->cmds[i].io[0]);
		if (cmd_list->cmds[i].io[1] >= 0)
			close(cmd_list->cmds[i].io[1]);
		i++;
	}
	return (1);
}
