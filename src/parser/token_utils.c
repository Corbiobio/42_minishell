/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:00:52 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/29 17:48:13 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_word(t_token token)
{
	if (token.type == TYPE_WORD)
		return (1);
	else if (token.type == TYPE_WORD_QUOTED)
		return (1);
	return (0);
}

int	is_quote(t_token token)
{
	if (token.type == TYPE_SINGLE_QUOTE)
		return (1);
	else if (token.type == TYPE_DOUBLE_QUOTE)
		return (1);
	return (0);
}

void	add_token(t_tokenized_line *line, t_token token_to_add)
{
	line->tokens[line->nb_token] = token_to_add;
	line->nb_token++;
}

int	is_type_redirect(t_token token)
{
	if (token.type == TYPE_LESSER)
		return (1);
	if (token.type == TYPE_GREATER)
		return (1);
	if (token.type == TYPE_LESSER_LESSER)
		return (1);
	if (token.type == TYPE_GREATER_GREATER)
		return (1);
	return (0);
}
