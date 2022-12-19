/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchahid <hchahid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:52:43 by hchahid           #+#    #+#             */
/*   Updated: 2022/12/17 18:05:38 by hchahid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**prepare_pipe(char **cmd)
{
	int	**fd;
	int	p_num;
	int	i;

	i = -1;
	p_num = arg_len(cmd) - 1;
	fd = malloc (p_num * sizeof(int *));
	if (!fd)
		return (printf("issue allocating space\n"), NULL);
	while (++i < p_num)
	{
		fd[i] = malloc (2 * sizeof(int ));
		if (!fd[i])
			return (printf("issue allocating space\n"), NULL);
	}
	i = -1;
	while (++i < p_num)
	{
		if (pipe (fd[i]) < 0)
			return (printf("issue in function pipe\n"), NULL);
	}
	return (fd);
}

void	free_fd(int **fd, char **ag)
{
	int	p_num;
	int	i;

	i = 0;
	p_num = arg_len(ag) - 1;
	while (i < p_num)
		free(fd[i++]);
	free(fd);
}

void	free_dp(char **s)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
		free(s[i++]);
	free(s);
}

void	fill_t_fd(t_fd *fd_pipe, int in, int out, int rest)
{
	fd_pipe->in = in;
	fd_pipe->out = out;
	fd_pipe->rest = rest;
}
