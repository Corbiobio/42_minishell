/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:32:50 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/29 11:11:47 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include <stddef.h>
# include "libft.h"

typedef struct s_cmd
{
	int		io[2];
	char	**cmd;
}	t_cmd;
//-1 in IO indicates error while opening file, -2 indicates no file provided

typedef struct s_cmd_list
{
	size_t	nb_cmd;
	t_cmd	cmds[];
}	t_cmd_list;

typedef enum e_error
{
	ERROR_UNKNOWN,
	ERROR_QUOTE_UNCLOSED,
	ERROR_REDIRECTION_NO_FILENAME,
	ERROR_HEREDOC_EOF
}	t_error;

void			free_cmd_list(t_cmd_list *list);
struct termios	set_signal_handler_main(void);
t_hash_table	*convert_env_to_table(char **env);

//error
int				print_error_set_status(t_error error, t_hash_table *env);
int				print_error_dont_set_status(t_error error);
int				perror_set_status(t_hash_table *env, int status, char *optional);
int				set_status(t_hash_table *env, int status);

#endif //MINISHELL_H
