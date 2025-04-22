/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:32:46 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/22 15:32:51 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"
#include "../libft/libft.h"
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

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

char	*alloc_word(t_tokenized_line *line, int token_index)
{
	char			*word;
	const int		len_token = line->tokens[token_index].len;
	const int		pos_token = line->tokens[token_index].pos;

	word = malloc(sizeof(char) * (len_token + 1));
	if (word == 0)
		return (0);
	ft_strlcpy(word, &line->line[pos_token], len_token + 1);
	return (word);
}

void	redirect_out(t_token redirect, char *filename, t_cmd *cmd)
{
	int	fd;

	if (redirect.type == TYPE_GREATER)
		fd = open(filename, O_TRUNC | O_WRONLY | O_CREAT, 0644);
	if (redirect.type == TYPE_GREATER_GREATER)
		fd = open(filename, O_APPEND | O_WRONLY | O_CREAT, 0644);
	cmd->io[1] = fd;
}

void	redirect_in(t_token redirect, char *filename, t_cmd *cmd)
{
	int	fd;

	if (redirect.type == TYPE_LESSER)
		fd = open(filename, O_RDONLY);
	else
		fd = open_heredoc(filename);
	cmd->io[0] = fd;
}

void	redirect_io(t_tokenized_line *line, int token_index, t_cmd *cmd)
{
	const t_token	redirect = line->tokens[token_index];
	char			*filename;

	filename = alloc_word(line, token_index + 1);
	if (redirect.type == TYPE_GREATER || redirect.type == TYPE_GREATER_GREATER)
		redirect_out(redirect, filename, cmd);
	else
		redirect_in(redirect, filename, cmd);
	free(filename);
}

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
		if (line->tokens[token_index + 1].type == TYPE_WORD)
			return (1);
	}
	return (0);
}

void	open_infile_outfile(t_tokenized_line *line, t_cmd_list *cmd_list)
{
	size_t	i;
	size_t	cmd_index;

	i = 0;
	cmd_index = 0;
	print_tokens(line);
	while (i < line->nb_token)
	{
		if (line->tokens[i].type == TYPE_PIPE)
			cmd_index++;
		if (is_type_redirect(line->tokens[i]) == 1 && next_token_is_word(line, i) == 1)
		{
			if (file_opening_did_not_fail(cmd_list->cmds[cmd_index]))
				redirect_io(line, i, &(cmd_list->cmds[cmd_index]));
		}
		else
			printf("ERROR_UNEXPECTED_TOKEN");
		i++;
	}
}
