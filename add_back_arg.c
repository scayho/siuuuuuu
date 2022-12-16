/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_back_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchahid <hchahid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:29:11 by hchahid           #+#    #+#             */
/*   Updated: 2022/09/25 17:33:00 by hchahid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/libft.h"

int	add_back_arg(t_arg **lst, t_arg *new)
{
	t_arg	*iter;

	iter = *lst;
	if (!new)
		return (1);
	if (*lst == NULL)
		*lst = new;
	else
	{
		last_arg(iter)->next = new;
	}
	return (0);
}