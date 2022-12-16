/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env_buf.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchahid <hchahid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 17:33:26 by hchahid           #+#    #+#             */
/*   Updated: 2022/11/01 03:03:49 by hchahid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *sub_str(char *s, int start, int end)
{
	char	*p;
	int		i;

	i = 0;
	p = malloc (end - start + 1);
	if (!p)
		perror (p);
	while (start <= end)
		p[i++] = s[start++];
	p[i] = '\0';
	return (p);
}

char	*remove_arg(char *buf, int start, int end)
{
	char	*ret;
	int		i;
	int		j;

	i = 0;
	j = 0;
	ret = malloc (ft_strlen(buf) - (end - start + 2));
	if (!ret)
		return (NULL);
	while (i < start - 1)
	{
		
		ret[i] = buf[i];
		i++;
	}
	j = i;
	i = end + 1;
	while (buf[i])
		ret[j++] = buf[i++];
	ret[j] = '\0';
	return (ret);
}

int	env_exist(t_env **env_p, char *name)
{
	t_env	*iter;

	iter = *env_p;
	if (!env_p || !iter)
		return (0);
	while (iter)
	{
		if (!ft_strcmp(name, iter->e_name))
			return (1);
		iter = iter->next;
	}
	return (0);
}

char	*expand_it(char *buf, char *tmp, int start, int end, t_env **env_p)
{
	char	*ret;
	char	*env;
	int 	len;
	int		i;
	int		j;

	i = -1;
	j = 0;
	env = extract_evar_value(*env_p, tmp);
	len = ft_strlen (buf) + end - start + 1 + ft_strlen (env);
	ret = malloc (len + 1);
	if (!ret)
		return (NULL);
	while (++i < start - 1)
		ret[i] = buf[i];
	while (env[j])
		ret[i++] = env[j++];
	j = end + 1;
	while (buf[j])
		ret[i++] = buf[j++];
	ret[i] = '\0';
	return (ret);
}


char	*replace_exstat_val(char *buf, int	start, int finish)
{
	char	*r;
	// int		i;

	// i = 0;
	buf[start] = 0;
	r = ft_strdup(buf);
	if (!r)
		return (perror(r), NULL);
	r = join_free(r, ft_itoa(g_var.exit_status));
	if (!r)
		return (perror(r), NULL);
	r = join_free(r, buf + finish);
	if (!r)
		return (perror(r), NULL);
	return (r);
}

char	*replace_env(char *buf, t_env **env)
{
	char	*tmp;
	char	*hold;
	// char	*wild;
	// char	*wild_free;
	int		index;
	int		i;
	
	i = 0;
	// printf("[%s]\n", buf);
	while (buf[i])
	{
		
		if (buf[i] == '$' && buf[i + 1] == '?' && (!ft_isalnum(buf[i + 2]) || buf[i + 2] != '_'))
		{
			hold = buf;
			buf = replace_exstat_val(buf, i , i + 2);
			i += ft_strlen(ft_itoa(g_var.exit_status));
			free(hold);
		}
		if (buf[i] == '$' && buf[i + 1] && buf[i + 1] != ' ')
		{
			// printf("suuu\n");
			i++;
			index = i;
			while (buf[i] && (ft_isalnum(buf[i]) || buf[i] == '_'))
				i++;
			tmp = sub_str(buf, index, i - 1);
			if (!tmp)
				return (perror(tmp), NULL);
			else if  (env_exist(env, tmp))
			{
				hold = buf;
				buf = expand_it(buf, tmp, index, i - 1, env);
				i = index;
				i += ft_strlen(extract_evar_value(*env, tmp));
				free(hold);
			}
			else
			{
				hold = buf;
				buf = remove_arg(buf, index, i - 1);
				i = index - 1;
				free(hold);
			}
			free(tmp);
			i -= 1;
		}
		// if (buf[i] == '*' && (is_space(buf[i - 1]) || !buf[i - 1]) && (is_space(buf[i + 1]) || !buf[i + 1]) && !is_quoted(buf, &buf[i]))
		// {
        //     wild = wildcard();
        //     buf[i] = '\0';
        //     wild_free = buf;
        //     buf = ft_strjoin(buf, wild);
        //     // free(wild);
        //     buf = ft_strjoin(buf, wild_free + i + 1);
        //     free(wild_free);
        //     i += ft_strlen(wild);
        //     free(wild);
		// }
		i++;
	}
	return(buf);
}

char	*full_replace_env(char *buf, t_env **env)
{
	char	*tmp;
	char	*hold;
	char	*wild;
	char	*wild_free;
	int		index;
	int		i;
	
	i = 0;
	// printf("[%s]\n", buf);
	// buf = ft_strjoin(" ", buf);
	// buf = ft_strjoin(buf, " ");
	while (buf[i])
	{
		
		if (buf[i] == '$' && buf[i + 1] == '?' && (!ft_isalnum(buf[i + 2]) || buf[i + 2] != '_'))
		{
			hold = buf;
			buf = replace_exstat_val(buf, i , i + 2);
			i += ft_strlen(ft_itoa(g_var.exit_status));
			free(hold);
		}
		if (buf[i] == '$' && buf[i + 1] && buf[i + 1] != ' ')
		{
			// printf("suuu\n");
			i++;
			index = i;
			while (buf[i] && (ft_isalnum(buf[i]) || buf[i] == '_'))
				i++;
			tmp = sub_str(buf, index, i - 1);
			if (!tmp)
				return (perror(tmp), NULL);
			else if  (env_exist(env, tmp))
			{
				hold = buf;
				buf = expand_it(buf, tmp, index, i - 1, env);
				i = index;
				i += ft_strlen(extract_evar_value(*env, tmp));
				free(hold);
			}
			else
			{
				hold = buf;
				buf = remove_arg(buf, index, i - 1);
				i = index - 1;
				free(hold);
			}
			free(tmp);
			i -= 1;
		}
		if (buf[i] == '*' && i == 0 && (is_space(buf[i + 1]) || !buf[i + 1]) && !is_quoted(buf, &buf[i]))
		{
            wild = wildcard();
            buf[i] = '\0';
            wild_free = buf;
            buf = ft_strjoin(buf, wild);
            free(wild_free);
            wild_free = buf;
            buf = ft_strjoin(buf, wild_free + i + 1);
            free(wild_free);
            i += ft_strlen(wild);
            free(wild);
		}
		// else if (buf[i] == '*' && (is_space(buf[i - 1]) || !buf[i - 1]) && (is_space(buf[i + 1]) || !buf[i + 1]) && !is_quoted(buf, &buf[i]))
		// {
        //     wild = wildcard();
        //     buf[i] = '\0';
        //     wild_free = buf;
        //     buf = ft_strjoin(buf, wild);
        //     // free(wild);
        //     buf = ft_strjoin(buf, wild_free + i + 1);
        //     free(wild_free);
        //     i += ft_strlen(wild);
        //     free(wild);
		// }
		i++;
	}
	return(buf);
}
