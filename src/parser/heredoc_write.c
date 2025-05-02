/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_write.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:54:28 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/02 14:10:37 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	write_no_expand_heredoc(char *line, int write_end)
{
	write(write_end, line, ft_strlen(line));
	write(write_end, "\n", 1);
	free(line);
}

void	write_expander_heredoc(char *line, int write_end,
						t_free_close *stuff, char *eof)
{
	t_tokenized_line	*tokens;

	tokens = expander(line, stuff->env);
	if (tokens == 0)
	{
		close(write_end);
		free_2(line, eof);
		delete_all_heredoc(stuff);
		exit(EXIT_FAILURE);
	}
	write(write_end, tokens->line, ft_strlen(tokens->line));
	write(write_end, "\n", 1);
	free_2(tokens->line, tokens);
}
