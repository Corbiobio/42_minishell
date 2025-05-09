/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:43:41 by edarnand          #+#    #+#             */
/*   Updated: 2025/05/09 15:10:49 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "execution.h"

//echo
void	ft_echo(t_cmd cmd, int *status);

//export
void	ft_export(t_cmd cmd, t_hash_table *env, int *status);

//env
void	ft_env(t_cmd cmd, t_hash_table *env, int *status);

//unset
void	ft_unset(t_cmd cmd, t_hash_table *env, int *status);

//exit
int		ft_exit(t_cmd cmd, int *status, t_position pos, int stds[2]);

//pwd
void	ft_pwd(int *status);

//cd
void	ft_cd(t_cmd cmd, t_hash_table *env, int *status);

#endif
