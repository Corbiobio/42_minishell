/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:32:33 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/22 10:05:27 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H

# define PARSER_H

# include "minishell.h"
# include <stddef.h>

typedef enum e_type
{
	TYPE_ERROR,
	TYPE_WORD,
	TYPE_WORD_SINGLE_QUOTE,
	TYPE_WHITESPACE,
	TYPE_SINGLE_QUOTE,
	TYPE_DOUBLE_QUOTE,
	TYPE_GREATER,
	TYPE_LESSER,
	TYPE_GREATER_GREATER,
	TYPE_LESSER_LESSER,
	TYPE_PIPE,
}	t_type;

typedef struct s_token
{
	t_type	type;
	size_t	pos;
	size_t	len;
}	t_token;

typedef struct s_tokenized_line
{
	char	*line;
	size_t	nb_token;
	t_token	tokens[];
}	t_tokenized_line;

t_tokenized_line	*lexer(char *line);
void				add_token(t_tokenized_line *line, t_token token_to_add);

void				open_infile_outfile(t_tokenized_line *line, t_cmd_list *cmd_list);

void				grammarify(t_tokenized_line *line, t_cmd_list *cmd_list);

char				*alloc_word(t_tokenized_line *line, int token_index);

int					is_type_redirect(t_token token);

int					create_heredoc(char *eof);

void	print_tokens(t_tokenized_line *line);
void	print_cmds(t_cmd_list *list);

#endif //PARSER_H
