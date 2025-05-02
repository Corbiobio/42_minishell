/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:32:33 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/02 14:10:55 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H

# define PARSER_H

# include "minishell.h"
# include <stddef.h>
# include "../libft/libft.h"

typedef enum e_infile
{
	INFILE_NORMAL,
	INFILE_HEREDOC_EXPAND,
	INFILE_HEREDOC_DONT
}	t_infile;

typedef enum e_type
{
	TYPE_ERROR,
	TYPE_WORD,
	TYPE_DEAD_TOKEN,
	TYPE_WHITESPACE,
	TYPE_SINGLE_QUOTE,
	TYPE_DOUBLE_QUOTE,
	TYPE_GREATER,
	TYPE_LESSER,
	TYPE_GREATER_GREATER,
	TYPE_LESSER_LESSER,
	TYPE_PIPE,
	TYPE_DOLLAR,
	TYPE_WORD_QUOTED
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

typedef struct s_free_close
{
	int					fd_read_end;
	t_tokenized_line	*line1;
	char				*line2;
	t_cmd_list			*cmds;
	t_hash_table		*env;
}	t_free_close;

//lexer.c
t_tokenized_line	*expander(char *line, t_hash_table *env);
t_tokenized_line	*lexer(t_tokenized_line *input);
void				add_token(t_tokenized_line *line, t_token token_to_add);
void				tokenize_string(char *line, t_tokenized_line *tokens);
int					is_word(t_token token);

//lexer_make_words.c
void				fuse_words(t_tokenized_line *input,
						t_tokenized_line *output);
void				remove_whitespaces(t_tokenized_line *input,
						t_tokenized_line *output);
void				fuse_chevrons(t_tokenized_line *in, t_tokenized_line *out);

//expander.c
void				tokenize_string(char *line, t_tokenized_line *tokens);
int					turn_quoted_tokens_to_word(t_tokenized_line *line,
						t_hash_table *env);
void				dollar_alone_is_dead(t_tokenized_line *line);
int					is_allowed_in_variable_name(t_tokenized_line *line,
						int token_index);
int					variable_name_start_correctly(t_tokenized_line *line,
						size_t token_index);

//expander_search.c
char				*search_and_replace(t_tokenized_line *line,
						t_hash_table *env);
void				fuse_dollars(t_tokenized_line *input,
						t_tokenized_line *output);

//expander_core.c
void				correct_positions(t_tokenized_line *line,
						size_t new_len, size_t old_len, size_t index);
void				expand_token_list(t_tokenized_line *input,
						t_tokenized_line *output);
int					expand_variables(t_tokenized_line *input,
						t_tokenized_line *intermediary, t_hash_table *env);

//io.c
int					open_infile_outfile(t_tokenized_line *line,
						t_cmd_list *cmd_list, t_free_close *to_free);
int					close_all_fd(t_cmd_list *cmd_list);

//io_conditions.c
int					file_opening_did_not_fail(t_cmd cmd);
int					next_token_is_word(t_tokenized_line *line,
						size_t token_index);
int					close_all_fd(t_cmd_list *cmd_list);

//grammar.c
int					grammarify(t_tokenized_line *line, t_cmd_list *cmd_list);

//heredoc.c
int					create_heredoc(char *eof, t_free_close *stuff,
						t_infile how_expand);
void				delete_all_heredoc(t_free_close *stuff);

//heredoc_signals.c
void				set_signal_handler_parent(void);
void				set_signal_handler_heredoc(void);

//heredoc_write.c
void				write_no_expand_heredoc(char *line, int write_end);
void				write_expander_heredoc(char *line, int write_end,
						t_free_close *stuff, char *eof);

//token_utils.c
int					is_type_redirect(t_token token);
int					is_word(t_token token);
int					is_quote(t_token token);
void				add_token(t_tokenized_line *line, t_token token_to_add);
void				change_last_token_type(t_tokenized_line *line, t_type type);

//parser.c
t_cmd_list			*parser(char *line, t_hash_table *env);

char				*alloc_word(t_tokenized_line *line, int token_index);

int					is_type_redirect(t_token token);

void				print_tokens(t_tokenized_line *line);
void				print_cmds(t_cmd_list *list);

#endif //PARSER_H
