/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchahid <hchahid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 18:46:16 by hchahid           #+#    #+#             */
/*   Updated: 2022/11/02 17:27:56 by hchahid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>

void	fake_herdoc(char	*delemeter)
{
	char	*string;

	string = readline("herdoc >: ");
	while(ft_strcmp(string, delemeter))
		string = readline("herdoc >: ");
}

void	herdoc(char	*delemeter, int fd)
{
	char	*string;

	string = readline("heredoc >: ");
	while(ft_strcmp(string, delemeter))
	{
		ft_putstr_fd(string, fd);
		ft_putstr_fd("\n", fd);
		string = readline("heredoc >:");
	}
}
// cat <<""'    '""ko<<""''"'" < zebzabi
void	extande_heredoc(char *buf)
{
	int i;
	char *name;

	i = 0;
	while (first_heredoc(buf, 1) != -1)
	{
		i = first_heredoc(buf, 1);
		name = redirection_name(ft_strdup(buf + i + 2));
		buf = exlude_name(buf, i , 2);
		fake_herdoc(name);
	}
}

char *get_redi(t_arg *command)
{
	char	*buf;
	t_arg	*iter;

	iter = command;

	buf = ft_strdup("");
	while (iter)
	{
		if (iter->redirection)
		{
			buf = join_free(buf, iter->arg);
			buf = join_free(buf, " ");
			del_arg(&command, iter);
		}
		iter = iter->next;
	}
	return (buf);
}

char *get_file_names(void)
{
	struct dirent *file_dir;
	char *name;
	char **files;
	DIR *dir;
	int i;
	int j;
	char *extra;
	char *ex;

	i = 0;
	extra = malloc (2);
	extra[0] = 'A';
	extra[1] = '\0';
	j = 0;
	dir = opendir("/tmp/");
	if (!dir)
	{
		write(2, "Opendir issue\n", 14);
		return (NULL);
	}
	file_dir = readdir(dir);
	if (!file_dir)
		return (NULL);
	name = ft_strdup(".A");
	while (file_dir)
	{
		file_dir = readdir(dir);
		i++;
	}
	closedir(dir);
	dir = opendir("/tmp/");
	if (!dir)
	{
		write(2, "Opendir issue\n", 14);
		return (NULL);
	}
	files = malloc(sizeof(char *) * i + 1);
	if (!files)
		printf("tableau d zab mabrach alloca\n");
	file_dir = readdir(dir);
	while(file_dir)
	{
		files[j++] = file_dir->d_name;
		file_dir = readdir(dir);
	}
	files[j] = NULL;
	j = 0;
	i = 0;
	while (files[j])
	{
		if (ft_strcmp(files[j], name))
			j++;
		else
		{
			name = join_free(name, extra);
			i++;
			j = 0;
		}
	}
	free(files);
	closedir(dir);
	ex = name;
	name = ft_strjoin("/tmp/", name);
	free(ex);
	free(extra);
	return (name);
}

char	*exec_redirection_pipe(t_arg *command, char **buf, t_env **env_p, int *pipe_redi)
{
	(void)env_p;
	(void)pipe_redi;
	char	*name;
	char *cmd;
	int		i;
	char *file;
	// char *cmd;
	// int		j;
	// int		heredoc_fd;
	int		next_heredoc;
	// int	*input;
	int	input1;
// 	t_redi *redirection;
	// int j = 0;

	i = 0;
	cmd = *buf;
	cmd = get_redi(command);
	next_heredoc = 0;
	input1 = 0;
	name = NULL;
	while (first_redirection(cmd) != -1)
	{
		// if (dup2(STDIN_FILENO, g_var.dup_in) == -1)
		// 	dprintf(2, "failat stdin a zabi \n");
		// if (dup2(STDOUT_FILENO, g_var.dup_out) == -1)
		// 	dprintf(2, "failat stdout a zabi \n");
		// dprintf(2, "bono ya bo no ache bari dir fl mgharba \n");
		// close(STDIN_FILENO);
		// close(STDOUT_FILENO);
		if (first_heredoc(cmd, 2) != -1)
		{
			i = first_heredoc(cmd, 2);
		}
		else
			i = first_redirection(cmd);
		if (cmd[i] == '>' && cmd[i + 1] == '>')
		{
			name = redirection_name(ft_strdup(cmd + i + 2));
			cmd = exlude_name(cmd, i, 2);
			redirect_output(name, O_APPEND);
			free(name);
		}
		else if (cmd[i] == '>')
		{
			name = redirection_name(ft_strdup(cmd + i + 1));
			cmd = exlude_name(cmd, i , 2);
			redirect_output(name, O_TRUNC);
			free(name);
		}
		if (cmd[i] == '<' && cmd[i + 1] == '<')
		{
			name = redirection_name(ft_strdup(cmd + i + 2));
			dprintf(2, "<<<<<<[%s] :%s\n", name, cmd + i);
			next_heredoc = last_red_in(cmd, cmd + i);
			cmd = exlude_name(cmd, i , 2);
			dprintf(2, ">[%s] :%s\n", name, cmd + i);
			if (next_heredoc)
				fake_herdoc(name);
			else if (!next_heredoc)
			{
				file = get_file_names();
				input1 = open(file, O_CREAT | O_RDWR | O_TRUNC , 0664);
				// input1 = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC , 0777);
				herdoc(name, input1);
				close(input1);
				input1 = open(file, O_CREAT | O_RDWR , 0664);
				if(dup2(input1, STDIN_FILENO) == -1)
				{
					dprintf(2, "UUUUUUUfailed to dup2 \n");
					exit(0);
				}
				close(input1);
			}
			free(name);
		}
		else if (cmd[i] == '<')
		{
			name = redirection_name(ft_strdup(cmd + i + 1));
			next_heredoc = last_red_in(cmd, cmd + i);
			if (open(name, O_RDONLY) == -1)
			{
				cmd = exlude_name(cmd, i , 0);
				extande_heredoc(cmd);
				dprintf(2, "%s\n", cmd);
				dprintf(2, "%s : no such file or directory\n", name);
				exit(1);
			}
			else
				cmd = exlude_name(cmd, i , 2);
			if (!next_heredoc)
			{
				input1 = open(name, O_RDONLY, 0664);
				if (input1 < 0)
					;
				else if(dup2(input1, STDIN_FILENO) == -1)
				{
					dprintf(2, "failed to dup2 \n");
					exit(0);
				}
				close(input1);
			}
			free(name);
		}
	}
	free(cmd);
	// cmd = get_not_redi(command);
	cmd = join_list(command);
	// dprintf(2, ">>>>> %s\n", cmd);
	// printf("not redirections : [%s]\n", cmd);
	free(*buf);
	*buf = cmd;
	// cmd = eraseqout(cmd, markqout(cmd));
	// system(ft_strjoin("leaks ", ft_itoa(getpid())));
	// execute_redirection_pipe(cmd, env_p, file);
	return(file);
}

void	redirection(t_arg *command, char *buf, t_env **env_p)
{
	(void)env_p;
	char	*name;
	int		i;
	char *file;
	// char *cmd;
	// int		j;
	// int		heredoc_fd;
	int		next_heredoc;
	// int	*input;
	int	input1;
	int pid;
// 	t_redi *redirection;
	// int j = 0;

	i = 0;
	next_heredoc = 0;
	input1 = 0;
	name = NULL;
	pid = fork();
	if (pid == -1)
	{
		printf("\033[0;31mUnable to create processe\n");
		return ;
	}
	else if (pid == 0)
	{
		buf = get_redi(command);
		if (input1 == -1)
		{
			printf("97ab f %d a zabi\n", i);
			exit(0);
		}
		while (first_redirection(buf) != -1)
		{
			if (first_heredoc(buf, 2) != -1)
			{
				i = first_heredoc(buf, 2);
			}
			else
				i = first_redirection(buf);
			if (buf[i] == '>' && buf[i + 1] == '>')
			{
				name = redirection_name(ft_strdup(buf + i + 2));
				buf = exlude_name(buf, i, 2);
				redirect_output(name, O_APPEND);
				free(name);
			}
			else if (buf[i] == '>')
			{
				name = redirection_name(ft_strdup(buf + i + 1));
				buf = exlude_name(buf, i , 2);
				redirect_output(name, O_TRUNC);
				free(name);
			}
			if (buf[i] == '<' && buf[i + 1] == '<')
			{
				name = redirection_name(ft_strdup(buf + i + 2));
				next_heredoc = last_red_in(buf, buf + i);
				buf = exlude_name(buf, i , 2);
				if (next_heredoc)
					fake_herdoc(name);
				else if (!next_heredoc)
				{
					file = get_file_names();
					input1 = open(file, O_CREAT | O_RDWR | O_TRUNC , 0664);
					herdoc(name, input1);
					close(input1);
					input1 = open(file, O_CREAT | O_RDWR , 0664);
					if(dup2(input1, STDIN_FILENO) == -1)
					{
						printf("failed to dup2 \n");
						exit(0);
					}
					close(input1);
				}
				// free(file);
				free(name);
			}
			else if (buf[i] == '<')
			{
				name = redirection_name(ft_strdup(buf + i + 1));
				next_heredoc = last_red_in(buf, buf + i);
				if (open(name, O_RDONLY) == -1)
				{
					buf = exlude_name(buf, i , 0);
					extande_heredoc(buf);
					printf("%s\n", buf);
					printf("%s : no such file or directory\n", name);
					exit(1);
				}
				else
					buf = exlude_name(buf, i , 2);
				if (!next_heredoc)
				{
					input1 = open(name, O_RDONLY, 0664);
					if (input1 < 0)
						;
					else if(dup2(input1, STDIN_FILENO) == -1)
					{
						printf("failed to dup2 \n");
						exit(0);
					}
					close(input1);
				}
				free(name);
			}
		}
		free(buf);
		buf = join_list(command);
		execute_redirection(buf, env_p, file);
		exit(0);
	}
	sigInit();
	waitpid(pid, &(g_var.exit_status), 0);
	check_exit_status(pid);
}

int	redirect_output(char *name, unsigned int open_flag)
{
	int	file;

	file = open (name, O_RDWR |  O_CREAT | open_flag, 0664);
	if (file == -1)
		exit (printf("llllIssue opening file1\n"));
	if (dup2(file, STDOUT_FILENO) == -1)
	{
		exit (printf("Issue opening file2\n"));
	}
	close(file);
	return (0);
}

int	redirect_input(char *name)
{
	int	file;

	file = open (name, O_RDWR , 0664);
	if (file == -1)
	{
		ft_putstr_fd("Issue opening file3\n", 2);
		exit (1);
	}
	if (dup2(file, STDIN_FILENO) == -1)
	{
		ft_putstr_fd("Issue opening file4\n", 2);
		exit (1);
	}
	close(file);
	return (0);
}
// ################################################################################################################################################################################################
// void	fake_herdoc(char	*delemeter)
// {
// 	char	*string;

// 	string = readline("fake_herdoc >: ");
// 	while(ft_strcmp(string, delemeter))
// 		string = readline("fake_herdoc >: ");
// }
// void	herdoc(char	*delemeter, int fd)
// {
// 	char	*string;

// 	string = readline("herdoc >: ");
// 	while(ft_strcmp(string, delemeter))
// 	{
// 		ft_putstr_fd(string, fd);
// 		ft_putstr_fd("\n", fd);
// 		string = readline("herdoc >: ");
// 	}
// }
// // cat <<""'    '""ko<<""''"'" < zebzabi
// void	extande_heredoc(char *buf)
// {
// 	int i;
// 	char *name;

// 	i = 0;
// 	while (first_heredoc(buf, 1) != -1)
// 	{
// 		i = first_heredoc(buf, 1);
// 		name = redirection_name(ft_strdup(buf + i + 2));
// 		printf("%s\n", name);
// 		buf = exlude_name(buf, i , 2);
// 		fake_herdoc(name);
// 	}
// }

// void	redirection(char *buf, t_env **env_p)
// {
// 	(void)env_p;
// 	t_redi *redirection;
// 	char	*name;
// 	int		i;
// 	// int		j;
// 	// int		heredoc_fd;
// 	// int		next_heredoc;
// 	// int	*input;
// 	// int	input1;
// 	int pid;
// 	// int j = 0;

// 	i = 0;
// 	// j = 0;
// 	redirection = malloc(sizeof(t_redi));
// 	if (!redirection)
// 	{
// 		printf("protex\n");
// 		exit(0);
// 	}
// 	redirection->next_heredoc = 0;
// 	redirection->input1 = 0;
// 	redirection->name = NULL;
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		printf("\033[0;31mUnable to create processe\n");
// 		return ;
// 	}
// 	else if (pid == 0)
// 	{
// 		if (redirection->input1 == -1)
// 		{
// 			printf("97ab f %d a zabi\n", i);
// 			exit(0);
// 		}
// 		while (first_redirection(buf) != -1)
// 		{
// 			if (first_heredoc(buf, 2) != -1)
// 			{
// 				redirection->i = first_heredoc(buf, 2);
// 			// if (first_heredoc(buf) != -1 || first_input(buf) != -1)
// 				// if (first_heredoc(buf) < first_input(buf) && first_heredoc(buf) > -1)
// 				// else if (first_heredoc(buf) > first_input(buf) && first_input(buf) > -1)
// 				// 	i = first_input(buf);
// 				// printf("bonour %d  %d  %d\n", first_heredoc(buf), first_input(buf), i);
// 			}
// 			else
// 				redirection->i = first_redirection(buf);
// 			if (buf[redirection->i] == '>' && buf[redirection->i + 1] == '>')
// 			{
// 				redirection->name = redirection_name(ft_strdup(buf + redirection->i + 2));
// 				buf = exlude_name(buf, redirection->i, 2);
// 				redirect_output(redirection->name, O_APPEND);
// 			}
// 			else if (buf[redirection->i] == '>')
// 			{
// 				redirection->name = redirection_name(ft_strdup(buf + redirection->i + 1));
// 				buf = exlude_name(buf, redirection->i , 2);
// 				redirect_output(redirection->name, O_TRUNC);
// 			}
// 			if (buf[redirection->i] == '<' && buf[redirection->i + 1] == '<')
// 			{
// 				redirection->name = redirection_name(ft_strdup(buf + redirection->i + 2));
// 				redirection->next_heredoc = last_red_in(buf, buf + redirection->i);
// 				buf = exlude_name(buf, redirection->i , 2);
// 				if (redirection->next_heredoc)
// 					fake_herdoc(redirection->name);
// 				else if (!redirection->next_heredoc)
// 				{
// 					redirection->input1 = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC , 0777);
// 					herdoc(redirection->name, redirection->input1);
// 					close(redirection->input1);
// 					redirection->input1 = open(".heredoc", O_CREAT | O_RDWR , 0777);
// 					if(dup2(redirection->input1, STDIN_FILENO) == -1)
// 					{
// 						printf("failed to dup2 \n");
// 						exit(0);
// 					}
// 				}
// 			}
// 			else if (buf[redirection->i] == '<')
// 			{
// 				redirection->name = redirection_name(ft_strdup(buf + redirection->i + 1));
// 				printf("name is : %s\n", redirection->name);
// 				redirection->next_heredoc = last_red_in(buf, buf + redirection->i);
// 				if (open(name, O_RDONLY) == -1)
// 				{
// 					buf = exlude_name(buf, redirection->i , 0);
// 					extande_heredoc(buf);
// 					printf("%s : no such file or directory\n", redirection->name);
// 					exit(1);
// 				}
// 				else
// 					buf = exlude_name(buf, redirection->i , 2);
// 				if (!redirection->next_heredoc)
// 				{
// 					redirection->input1 = open(redirection->name, O_RDONLY, 0777);
// 					if (redirection->input1 < 0)
// 						;
// 					else if(dup2(redirection->input1, STDIN_FILENO) == -1)
// 					{
// 						printf("failed to dup2 \n");
// 						exit(0);
// 					}
// 				} 
// 			}
// 			free(redirection->name);
// 		}
// 		execute(buf, env_p);
// 		unlink("/Users/scayho/Desktop/1337/mnsh/.heredoc");
// 		exit(0);
// 	}
// 	waitpid(pid, &(g_var.exit_status), 0);
// 	check_exit_status(pid);
// }
// int	redirect_output(char *name, unsigned int open_flag)
// {
// 	int	file;

// 	file = open (name, O_RDWR |  O_CREAT | open_flag, 0777);
// 	if (file == -1)
// 		exit (printf("Issue opening file1\n"));
// 	if (dup2(file, STDOUT_FILENO) == -1)
// 	{
// 		exit (printf("Issue opening file2\n"));
// 	}
// 	close(file);
// 	return (0);
// }

// int	redirect_input(char *name)
// {
// 	int	file;

// 	file = open (name, O_RDWR , 0777);
// 	if (file == -1)
// 	{
// 		ft_putstr_fd("Issue opening file3\n", 2);
// 		exit (1);
// 	}
// 	if (dup2(file, STDIN_FILENO) == -1)
// 	{
// 		ft_putstr_fd("Issue opening file4\n", 2);
// 		exit (1);
// 	}
// 	close(file);
// 	return (0);
// }
// ################################################################################################################################################
// int	heredoc(char *name)
// {
// 	char	*tmp;
// 	int	file;
// 	// int	fd[2];
// 	// char *join = ft_strdup("");

// 	// tmp = NULL;
// 	// pipe(fd);
// 	// name = ft_strjoin("/Users/hchahid/goinfre/", name);
// 	file = open ("/tmp/heredoc", O_RDWR |  O_CREAT |  O_APPEND | O_TRUNC, 0777);
// 	if (file == -1)
// 	{
// 		ft_putstr_fd("Issue opening heredoc\n", 2);
// 		exit (1);
// 	}
// 	while (1)
// 	{
// 		tmp = readline("\033[0;33m |HEREDOC| > ");
// 		if (!tmp || !ft_strcmp(tmp, name))
// 			break ;
// 		ft_putstr_fd(tmp, STDIN_FILENO);
// 	}
// 	if (dup2(file, STDIN_FILENO) == -1)
// 	{
// 		ft_putstr_fd("Issue duping heredoc\n", 2);
// 		exit (1); 
// 	}
// 	close(file);
// 	// while(1)
// 	// {
// 	// 	tmp = readline("\033[0;33m> ");
// 	// 	printf("the first readline is :{%s}\n", tmp);
// 	// 	if (!ft_strcmp(tmp, name))
// 	// 		break ;
// 	// 	tmp = ft_strjoin(tmp, "\n");
// 	// 	join = ft_strjoin(join, tmp);
// 	// }
// 	// ft_putstr_fd(tmp, fd[1]);
// 	// close(fd[1]);
// 	return (file);
// }

// int	append_from_output(char **splited, t_env **env_p)
// {
// 	int	file;
// 	int pid;
// 	int	i;
// 	int	j;

// 	j = 0;
// 	i = 0;
// 	while (splited[i])
// 		i++;
// 	pid = fork();
// 	if (pid == -1)
// 			return (printf("\033[0;31mUnable to create processe\n"));
// 	else if (pid == 0)
// 	{
// 		file = open (splited[i - 1], O_RDWR |  O_CREAT | O_APPEND, 0777);
// 		if (file == -1)
// 			exit (printf("Issue opening file\n"));
// 		if (dup2(file, STDOUT_FILENO) == -1)
// 			exit (printf("Issue opening file\n"));
// 		execute(splited[0], env_p);
// 		close(file);
// 		exit(0);
// 	}
// 	wait(NULL);
// 	return (0);
// } 