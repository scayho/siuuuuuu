/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchahid <hchahid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 13:26:32 by hchahid           #+#    #+#             */
/*   Updated: 2022/09/25 00:33:58 by hchahid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_env	*ft_lstnew(char *env_var)
{
	t_env	*new;
	char	*tmp;
	int		i;

	i = -1;
	// if (!ft_strchr(env_var, '='))
	// {
	// 	ft_putstr_fd("invalid identifier\n", 2);
	// 	return (NULL);
	// }
	tmp = NULL;
	new = malloc (sizeof (t_env));
	if (!new)
	{
		printf("\033[0;31mUnable to add enviroment variable due to allocation issues\n");
		return (NULL);
	}
	new->e_name = get_e_var_name(env_var);
	if (!(new->e_name))
		return (NULL);
	tmp = ft_strchr(new->e_name, '+');
	if (tmp)
		*tmp = '\0';
	new->e_value = get_e_var_value(env_var);
	if (!(new->e_value))
		return (NULL);
	new->next = NULL;
	// printf("e_name = %s | e_value = %s\n", new->e_name, new->e_value);
	// if (!ft_strncmp(new->e_name, "SHLVL", 6))
	// {
	// 	new->e_value = ft_itoa(ft_atoi(ft_strdup(new->e_value)) + 1);
	// }
	if (!ft_strncmp(new->e_name, "PATH", 5))
	{
		g_var.paths = ft_split(new->e_value, ':');
		while (g_var.paths[++i])
		{
			tmp = g_var.paths[i];
			g_var.paths[i] = ft_strjoin(g_var.paths[i], "/");
			free(tmp);
		}
	}
	return (new);
}
