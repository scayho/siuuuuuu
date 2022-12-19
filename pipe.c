/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelahce <abelahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:59:52 by hchahid           #+#    #+#             */
/*   Updated: 2022/12/19 17:13:20 by abelahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_child(t_fd fd_pipe, char *ag, t_env **env_p)
{
	int		pipe_redi[2];
	char	*file;
	bool	input_flag;
	bool	output_flag;

	file = NULL;
	input_flag = 0;
	output_flag = 0;
	if (first_redirection(ag) != -1)
	{
		if (first_heredoc(ag, 2) != -1)
			input_flag = 1;
		if (outputcheck(ag, 2) != -1)
			output_flag = 1;
		file = exec_redirection_pipe(get_args(ag, env_p),
				&ag, env_p, pipe_redi);
	}
	close(fd_pipe.rest);
	if (!input_flag)
		dup2(fd_pipe.in, 0);
	close (fd_pipe.in);
	if (!output_flag)
		dup2(fd_pipe.out, 1);
	close (fd_pipe.out);
	execute_redirection_pipe(ag, env_p, file, get_args(ag, env_p));
}

void	wait_redirection_in_pipe(char *file, char *ag, int pid)
{	
	file = get_file_names();
	sig_init();
	waitpid(pid, &(g_var.exit_status), 0);
	unlink(file);
	free(file);
	free(ag);
}

void	pipe_it(t_fd fd_pipe, char *ag, t_env **env_p)
{
	char	*file;
	int		pid;

	file = NULL;
	ag = ft_strtrim(ag, " ");
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("Error forking for pipe\n", 2);
		g_var.exit_status = 1;
		return ;
	}
	if (pid == 0)
		pipe_child(fd_pipe, ag, env_p);
	else if (pid && first_redirection(ag) != -1)
		wait_redirection_in_pipe(file, ag, pid);
	else
		free(ag);
	if (fd_pipe.out == -2)
	{
		waitpid(pid, &(g_var.exit_status), 0);
		check_exit_status(pid);
	}
}

void	pipe_loop(int **fd, int pnum, char **ag, t_env **env_p)
{
	t_fd	fd_pipe;
	int		i;

	i = 1;
	fill_t_fd(&fd_pipe, -1, fd[0][1], fd[0][0]);
	pipe_it (fd_pipe, ag[0], env_p);
	close(fd[0][1]);
	while (i <= pnum)
	{
		fill_t_fd(&fd_pipe, fd[i - 1][0], fd[i][1], fd[i][0]);
		pipe_it(fd_pipe, ag[i], env_p);
		close(fd[i - 1][0]);
		close(fd[i][1]);
		i++;
	}
	fill_t_fd(&fd_pipe, fd[i - 1][0], -2, fd[i -1][1]);
	pipe_it(fd_pipe, ag[i], env_p);
	close(fd[i - 1][0]);
	while (wait(NULL) != -1)
		;
}

int	do_pipe(char *p, t_env **env_p)
{
	char	**ag;
	int		**fd;

	ag = custom_split(p, '|');
	fd = prepare_pipe(ag);
	if (!fd)
		return (write(2, "Error preparing fd forking\n", 27));
	pipe_loop(fd, arg_len(ag) - 2, ag, env_p);
	free_fd(fd, ag);
	free_dp(ag);
	return (0);
}
