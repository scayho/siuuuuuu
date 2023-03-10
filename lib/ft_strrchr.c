/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchahid <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 12:49:09 by hchahid           #+#    #+#             */
/*   Updated: 2021/11/05 15:09:52 by hchahid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	p;

	i = 0;
	p = c;
	while (s[i])
		i++;
	while (p != s[i] && s[i] != s[0])
		i--;
	if (p == s[i])
		return ((char *)&s[i]);
	return (0);
}
