/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_conditions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:49:12 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/29 17:49:30 by sflechel         ###   ########.fr       */
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
