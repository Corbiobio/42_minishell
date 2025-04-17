/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:40:03 by sflechel          #+#    #+#             */
/*   Updated: 2024/11/07 17:41:48 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 'a' + 'A');
	return (c);
}

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c - 'A' + 'a');
	return (c);
}

char	*strjoin_and_free_s1(char *s1, char const *s2)
{
	char	*joined;
	size_t	len;

	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	joined = ft_calloc(len, sizeof(char));
	if (joined <= 0)
	{
		free(s1);
		return (0);
	}
	ft_strlcat(joined, s1, len);
	free(s1);
	ft_strlcat(joined, s2, len);
	return (joined);
}
