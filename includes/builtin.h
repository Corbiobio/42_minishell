/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:43:41 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/29 11:40:35 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "execution.h"
# include "minishell.h"

//echo
void	ft_echo(t_cmd cmd, int *status);

//export
void	ft_export(t_cmd cmd, t_hash_table *env, int *status);

//env
void	ft_env(t_cmd cmd, t_hash_table *env, int *status);

//unset
void	ft_unset(t_cmd cmd, t_hash_table *env, int *status);

//exit
void	ft_exit(int *status, t_position pos);

//pwd
void	ft_pwd(t_hash_table *env, int *status);

#endif
