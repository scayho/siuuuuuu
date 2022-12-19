/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nested_dirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelahce <abelahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 15:22:49 by hchahid           #+#    #+#             */
/*   Updated: 2022/12/19 19:07:41 by abelahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cut_pre_dir(char **dir)
{
	char	*ret;
	char	*d;
	int		j;
	int		i;

	i = 0;
	j = -1;
	d = *dir;
	if (!d)
		return ;
	while (d[i])
		i++;
	if (d[i - 1] == '/')
		i -= 2;
	while (d[i] != '/')
		i--;
	ret = malloc (i + 1);
	if (!ret)
		return (perror (ret));
	while (++j < i)
		ret[j] = d[j];
	ret[j] = '\0';
	free(d);
	*dir = ret;
}

void	retrieving_err(void)
{
	ft_putstr_fd("error retrieving current directory: ", 2);
	ft_putstr_fd("getcwd cannot access parent directories: ", 2);
	ft_putstr_fd("No such file or directory\n", 2);
}
