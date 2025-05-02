/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_token_bigger.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 16:10:16 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/02 16:10:27 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	make_token_bigger(	t_tokenized_line *line, size_t *token_index)
{
	(*token_index)++;
	line->tokens[line->nb_token - 1].len++;
}
