/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:32:46 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/27 12:06:46 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include "../../libft/libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

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

int	redirect_out(t_token redirect, char *filename, t_cmd *cmd)
{
	int	fd;

	if (cmd->io[1] >= 0)
		close(cmd->io[1]);
	if (redirect.type == TYPE_GREATER)
		fd = open(filename, O_TRUNC | O_WRONLY | O_CREAT, 0644);
	if (redirect.type == TYPE_GREATER_GREATER)
		fd = open(filename, O_APPEND | O_WRONLY | O_CREAT, 0644);
	cmd->io[1] = fd;
	return (fd);
}

int	redirect_in(t_token redirect, char *filename, t_cmd *cmd)
{
	int	fd;

	if (cmd->io[0] >= 0)
		close(cmd->io[0]);
	if (redirect.type == TYPE_LESSER)
		fd = open(filename, O_RDONLY);
	else
		fd = create_heredoc(filename);
	cmd->io[0] = fd;
	return (fd);
}

int	redirect_io(t_tokenized_line *line, int token_index, t_cmd *cmd)
{
	const t_token	redirect = line->tokens[token_index];
	char			*filename;
	int				error;

	filename = alloc_word(line, token_index + 1);
	if (filename == 0)
		return (-1);
	if (redirect.type == TYPE_GREATER || redirect.type == TYPE_GREATER_GREATER)
		error = redirect_out(redirect, filename, cmd);
	else
		error = redirect_in(redirect, filename, cmd);
	free(filename);
	return (error);
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

int	open_infile_outfile(t_tokenized_line *line, t_cmd_list *cmd_list)
{
	size_t	i;
	size_t	cmd_index;

	i = 0;
	cmd_index = 0;
	while (i < line->nb_token)
	{
		if (line->tokens[i].type == TYPE_PIPE)
			cmd_index++;
		if (is_type_redirect(line->tokens[i]) == 1)
		{
			if (next_token_is_word(line, i))
			{
				if (file_opening_did_not_fail(cmd_list->cmds[cmd_index]))
				{
					if (redirect_io(line, i, &(cmd_list->cmds[cmd_index])) <= 0)
						return (1);
				}
			}
			else
				return (print_error_return_one(ERROR_REDIRECTION_NO_FILENAME));
		}
		i++;
	}
	return (0);
}
