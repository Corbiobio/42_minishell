/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:32:46 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/30 14:23:27 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include "../../libft/libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

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

int	redirect_out(t_token redirect, char *filename,
			t_cmd *cmd, t_hash_table *env)
{
	int	fd;

	if (ft_strcmp(filename, "/dev/stdout") == 0)
		return (0);
	if (cmd->io[1] >= 0)
		close(cmd->io[1]);
	if (redirect.type == TYPE_GREATER)
		fd = open(filename, O_TRUNC | O_WRONLY | O_CREAT, 0644);
	if (redirect.type == TYPE_GREATER_GREATER)
		fd = open(filename, O_APPEND | O_WRONLY | O_CREAT, 0644);
	cmd->io[1] = fd;
	if (fd == -1)
		perror_set_status(env, 1, filename);
	return (0);
}

int	redirect_in(t_infile how_infile, char *filename,
				t_cmd *cmd, t_free_close *to_free)
{
	int	fd;

	if (ft_strcmp(filename, "/dev/stdin") == 0)
		return (0);
	if (cmd->io[0] >= 0)
		close(cmd->io[0]);
	if (how_infile == INFILE_NORMAL)
		fd = open(filename, O_RDONLY);
	else
		fd = create_heredoc(filename, to_free, how_infile);
	cmd->io[0] = fd;
	if (fd == -1)
	{
		if (how_infile == INFILE_NORMAL)
			perror_set_status(to_free->env, 1, filename);
		else
			return (-1);
	}
	return (0);
}

int	redirect_io(t_tokenized_line *line, int token_index,
				t_cmd *cmd, t_free_close *to_free)
{
	const t_token	redirect = line->tokens[token_index];
	char			*filename;
	int				error;
	t_infile		how_infile;

	if (redirect.type == TYPE_LESSER)
		how_infile = INFILE_NORMAL;
	else if (redirect.type == TYPE_LESSER_LESSER)
	{
		if (line->tokens[token_index + 1].type == TYPE_WORD)
			how_infile = INFILE_HEREDOC_EXPAND;
		else
			how_infile = INFILE_HEREDOC_DONT;
	}
	filename = alloc_word(line, token_index + 1);
	if (filename == 0)
		return (-1);
	if (redirect.type == TYPE_GREATER || redirect.type == TYPE_GREATER_GREATER)
		error = redirect_out(redirect, filename, cmd, to_free->env);
	else
		error = redirect_in(how_infile, filename, cmd, to_free);
	free(filename);
	return (error);
}

int	open_infile_outfile(t_tokenized_line *line,
						t_cmd_list *cmd_list, t_free_close *to_free)
{
	size_t	i;
	size_t	index;

	i = 0;
	index = 0;
	while (i < line->nb_token)
	{
		if (line->tokens[i].type == TYPE_PIPE)
			index++;
		if (!is_type_redirect(line->tokens[i]))
		{
			i++;
			continue ;
		}
		if (!next_token_is_word(line, i))
			return (print_error_set_status(ERROR_REDIRECTION_NO_FILENAME,
					to_free->env));
		if (file_opening_did_not_fail(cmd_list->cmds[index]))
			if (redirect_io(line, i, &(cmd_list->cmds[index]), to_free) == -1)
				return (1);
		i++;
	}
	return (0);
}
