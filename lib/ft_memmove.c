/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelahce <abelahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 21:23:53 by hchahid           #+#    #+#             */
/*   Updated: 2022/12/19 19:16:04 by abelahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*ls;
	char	*ld;

	if (!src && !dst)
		return (0);
	if (dst < src)
		ft_memcpy(dst, src, len);
	else
	{
		ls = (char *)src + (len - 1);
		ld = (char *)dst + (len - 1);
		while (len--)
			*ld-- = *ls--;
	}
	return (dst);
}
