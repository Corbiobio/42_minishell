/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:32:33 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/26 18:29:52 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H

# define PARSER_H

# include "minishell.h"
# include <stddef.h>
#include "../libft/libft.h"

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
	TYPE_DOLLAR
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

//lexer.c
t_tokenized_line	*expander(char *line, t_hash_table *env);
t_tokenized_line	*lexer(t_tokenized_line *input);
void				add_token(t_tokenized_line *line, t_token token_to_add);
void				tokenize_string(char *line, t_tokenized_line *tokens);

//expander.c
void				correct_positions(t_tokenized_line *line, size_t new_len, size_t old_len, size_t index);
void				expand_variables(t_tokenized_line *input, t_tokenized_line *intermediary, t_hash_table *env);
void				expand_token_list(t_tokenized_line *input, t_tokenized_line *output);

//io.c
int					open_infile_outfile(t_tokenized_line *line, t_cmd_list *cmd_list);

//grammar.c
void				grammarify(t_tokenized_line *line, t_cmd_list *cmd_list);

//heredoc.c
int					create_heredoc(char *eof);

//parser.c
t_cmd_list			*parser(char *line, t_hash_table *env);

char				*alloc_word(t_tokenized_line *line, int token_index);

int					is_type_redirect(t_token token);

void	print_tokens(t_tokenized_line *line);
void	print_cmds(t_cmd_list *list);

#endif //PARSER_H
