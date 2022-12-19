/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelahce <abelahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 15:59:05 by hchahid           #+#    #+#             */
/*   Updated: 2022/12/19 19:14:11 by abelahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**list_to_tab(t_env *env)
{
	char	**tab;
	t_env	*t;
	int		i;

	i = 0;
	t = env;
	while (env)
	{
		i++;
		env = env->next;
	}
	tab = malloc (sizeof(char *) * i);
	if (!tab)
		exit(0);
	i = 0;
	while (t)
	{
		tab[i] = ft_strjoin(ft_strjoin(t->e_name, "="), t->e_value);
		i++;
		t = t->next;
	}
	tab[i] = NULL;
	return (tab);
}

void	no_path(char **splited)
{
	printf("\033[0;31m %s: No such file or directory\n", splited[0]);
	free_dp(splited);
	exit(127);
}

void	binary_file(char **splited, t_env **env)
{
	char	**tb;

	tb = list_to_tab(*env);
	execve(splited[0], splited, tb);
	execve(splited[0], splited, NULL);
	printf("\033[0;31m %s: No such file or directory\n", splited[0]);
	free_dp(splited);
	exit(127);
}

void	check_for_binaries(char **splited, t_env **env)
{
	char	**tb;
	char	*cmd;
	int		i;

	i = 0;
	cmd = ft_strdup(splited[0]);
	free(splited[0]);
	while (g_var.paths[i])
	{
		splited[0] = ft_strjoin(g_var.paths[i], cmd);
		execve(splited[0], splited, NULL);
		free(splited[0]);
		i++;
	}
	splited[0] = ft_strjoin(g_var.pwd, "/");
	splited[0] = join_free(splited[0], cmd);
	tb = list_to_tab(*env);
	execve(splited[0], splited, tb);
	perror (cmd);
}

char	**prepar_tb_cmd(t_arg *arg)
{
	t_arg	*iter;
	int		i;
	char	**tb;

	i = 0;
	iter = arg;
	while (iter)
	{
		i++;
		iter = iter->next;
	}
	tb = malloc (sizeof(char *) * i + 1);
	if (!tb)
		return (ft_putstr_fd("failed to allocate \n", 2), NULL);
	iter = arg;
	i = 0;
	while (iter)
	{
		tb[i] = eraseqout(iter->arg, markqout(iter->arg));
		iter = iter->next;
		i++;
	}
	tb[i] = NULL;
	delete_arg(arg);
	return (tb);
}

char	**remove_qoutes_splited(char	**buf)
{
	int	i;

	i = 0;
	while (buf[i])
	{
		buf[i] = eraseqout(buf[i], markqout(buf[i]));
		if (!buf[i])
			return (NULL);
		i++;
	}
	return (buf);
}

void	cmd(t_arg *arg, t_env **env)
{
	char	**splited;

	splited = joincmd(arg);
	splited = remove_qoutes_splited(splited);
	if (!splited)
		return (ft_putstr_fd("error allocation", 2), exit(1));
	if (!splited[0])
		exit(1);
	if (!splited[0][0])
		return (ft_putstr_fd("command not found\n", 2), exit(127));
	if (if_directory(splited[0]))
		return (free_dp(splited), exit (126));
	else
	{
		if (!g_var.paths)
			no_path(splited);
		if (splited[0][0] == '/')
			binary_file(splited, env);
		else
			check_for_binaries(splited, env);
	}
	free_dp(splited);
	exit(127);
}

int	istherepipe(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == 1)
			i += ignore_alpha(s + i, 1);
		if (s[i] == '|' && !is_quoted(s, s + i))
			return (i);
		i++;
	}
	return (-1);
}

void	executables(char *buf, t_env **env_p)
{
	int	pid;

	g_var.exit_status = 0;
	pid = fork();
	if (pid == -1)
		printf("\033[0;31mUnable to create processe\n");
	else
	{
		if (pid == 0)
			cmd(get_args(buf, env_p), env_p);
		else
		{
			sig_init();
			waitpid(pid, &(g_var.exit_status), 0);
			check_exit_status(pid);
		}
	}
}

void	execute2(char *buf, t_env **env_p)
{
	t_arg	*arg;
	char	*file;

	file = NULL;
	arg = get_args(buf, env_p);
	if (isthereredirection(arg))
	{
		file = get_file_names();
		redirection(arg, buf, env_p);
	}
	else if (!built_in(buf, env_p, arg))
		;
	else
		executables(buf, env_p);
	unlink(file);
	free(file);
	free(buf);
}

int	execute(char *buf, t_env **env_p)
{
	int	pipecheck;

	sig_default();
	pipecheck = istherepipe(buf);
	if (pipecheck != -1)
	{
		do_pipe(buf, env_p);
		free(buf);
		return (0);
	}
	execute2(buf, env_p);
	sig_init();
	return (0);
}

int	execute_pipe(char *buf, t_env **env_p)
{
	if (!built_in(buf, env_p, get_args(buf, env_p)))
		;
	else
		cmd(get_args(buf, env_p), env_p);
	free(buf);
	return (0);
}

int	execute_redirection_pipe(char *buf, t_env **env_p, char *file, t_arg *arg)
{
	if (!buf)
		return (0);
	if (!built_in(buf, env_p, arg))
		;
	else
		cmd(get_args(buf, env_p), env_p);
	(void)file;
	free(buf);
	exit (0);
	return (0);
}

int	execute_redirection(char *buf, t_env **env_p, char *file)
{
	int	pid;

	(void)file;
	if (!buf)
		return (0);
	if (!built_in(buf, env_p, get_args(buf, env_p)))
		;
	else
	{
		g_var.exit_status = 0;
		pid = fork();
		if (pid == -1)
			return (printf("\033[0;31mUnable to create processe\n"));
		else
		{
			if (pid == 0)
				cmd(get_args(buf, env_p), env_p);
			else
			{
				waitpid(pid, &(g_var.exit_status), 0);
				check_exit_status(pid);
			}
		}
	}
	return (unlink(file), free(buf), 0);
}

char	*join_list(t_arg *cmd)
{
	char	*buf;
	t_arg	*iter;

	iter = cmd;
	if (!cmd)
		return (NULL);
	if (iter->linked)
		buf = ft_strdup(iter->arg);
	else
		buf = ft_strjoin(iter->arg, " ");
	iter = iter->next;
	while (iter)
	{
		buf = join_free(buf, iter->arg);
		if (!iter->linked && iter->next)
			buf = join_free(buf, " ");
		if (!buf)
			return (NULL);
		iter = iter->next;
	}
	delete_arg(cmd);
	return (buf);
}

void	init(t_env **env, char **envp)
{
	modify_attr();
	set_env_vars(envp, env);
	sig_init();
}

int	syntax(char *buf)
{
	if (buf[0] == '\0')
		return (1);
	if (check_qotes(buf) == 0)
		return (1);
	if (redirection_syntax(buf) == 1)
		return (printf("redirection error\n"));
	if (pipe_syntax(buf) == 1)
		return (printf("pipe error\n"));
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_env				*env_p;
	char				*buf;

	(void)ac;
	(void)av;
	env_p = NULL;
	buf = NULL;
	init(&env_p, envp);
	while (1)
	{
		buf = readline ("\033[0;34m(minishell) : \033[0;37m");
		if (!buf)
		{
			release_e_var(&env_p);
			exit(0);
		}
		else if (*buf)
			add_history(buf);
		if (syntax(buf))
		{
			free(buf);
			continue ;
		}
		execute(empty_string(buf), &env_p);
	}
}
