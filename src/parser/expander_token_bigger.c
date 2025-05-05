/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_token_bigger.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 16:10:16 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/04 17:16:00 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	make_token_bigger(	t_tokenized_line *line, size_t *token_index)
{
	(*token_index)++;
	line->tokens[line->nb_token - 1].len++;
}

void	quotes_to_empty_words(t_tokenized_line *input, t_tokenized_line *output)
{
	size_t	i;

	i = 0;
	*output = (t_tokenized_line){.line = input->line};
	while (i < input->nb_token)
	{
		add_token(output, input->tokens[i]);
		if (i + 1 < input->nb_token && is_quote(input->tokens[i]))
		{
			if (input->tokens[i].type == input->tokens[i + 1].type)
			{
				change_last_token_type(output, TYPE_WORD);
				output->tokens[output->nb_token - 1].len = 0;
				i++;
			}
		}
		i++;
	}
}
