/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelahce <abelahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 17:44:29 by hchahid           #+#    #+#             */
/*   Updated: 2022/12/19 19:17:38 by abelahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	mid_end_arg(t_arg **env_p, t_arg *var)
{
	t_arg	*iter;

	iter = *env_p;
	while (iter->next != var)
	{
		iter = iter->next;
	}
	iter->next = var->next;
	free(var->arg);
	free(var);
}

void	del_arg(t_arg **env_p, t_arg *var)
{
	t_arg	*iter;

	iter = *env_p;
	if (iter == var)
	{
		*env_p = (*env_p)->next;
		free(var->arg);
		free(var);
	}
	else
		mid_end_arg(env_p, var);
}

void	del(t_env **env_p, t_env *var)
{
	t_env	*iter;

	iter = *env_p;
	if (iter == var)
	{
		*env_p = (*env_p)->next;
		free(var->e_name);
		free(var->e_value);
		free(var);
	}
	else
		mid_end(env_p, var);
}

void	mid_end(t_env **env_p, t_env *var)
{
	t_env	*iter;

	iter = *env_p;
	while (iter->next != var)
	{
		iter = iter->next;
	}
	iter->next = var->next;
	free(var->e_name);
	free(var->e_value);
	free(var);
}
