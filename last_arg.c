/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchahid <hchahid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:27:52 by hchahid           #+#    #+#             */
/*   Updated: 2022/09/25 17:33:14 by hchahid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/libft.h"

t_arg	*last_arg(t_arg *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}