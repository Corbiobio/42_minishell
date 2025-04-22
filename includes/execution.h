/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:55:15 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/22 13:58:07 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_NAME
# define DEFINE_NAME

# include "minishell.h"

typedef enum e_position
{
	FIRST,
	MID,
	LAST
}	t_position;

void	create_child_and_exec_cmd(t_cmd_list *list, char **env);

#endif
