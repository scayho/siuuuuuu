#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
# include <stdbool.h>

#include "minishell.h"

// typedef struct s_arg
// {
// 	char			*arg;
// 	// bool			expand;
// 	bool			linked;
// 	struct s_arg	*next;
// }	t_arg;

// int	is_space(char c)
// {
// 	if (c == '\t' || c == '\n' || c == ' ' || c == '\v'
// 		|| c == '\f' || c == '\r')
// 		return (1);
// 	return (0);
// }
// int	ignore_alpha(char *str, int dj)
// {
// 	int i;

// 	i = 1;
// 	while (str[i] != dj && str[i] != '\0')
// 		i++;
// 	return (i + 1);
// }

// int	ft_strlen(const char *s)
// {
// 	int	i;

// 	i = 0;
// 	if (!s)
// 		return (0);
// 	while (s[i])
// 		i++;
// 	return (i);
// }

// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	char	*r;
// 	int		i;
// 	int		j;

// 	if (!s1 || !s2)
// 		return (0);
// 	i = ft_strlen(s1);
// 	j = ft_strlen(s2);
// 	r = malloc ((i + j + 1) * sizeof(char));
// 	if (!r)
// 		return (NULL);
// 	i = 0;
// 	while (s1[i])
// 	{
// 		r[i] = s1[i];
// 		i++;
// 	}
// 	j = 0;
// 	while (s2[j])
// 		r[i++] = s2[j++];
// 	r[i] = '\0';
// 	return (r);
// }

// char	*join_free(char const *s1, char const *s2)
// {
// 	char	*r;
    
//     r = ft_strjoin(s1, s2);
//     free((char *)s1);
//     return (r);
// }

// char	*ft_strdup(const char *s1)
// {
// 	int		i;
// 	char	*c;
// 	char	*p;

// 	i = 0;
// 	p = (char *)s1;
// 	if (!s1)
// 		return (0);
// 	while (s1[i])
// 		i++;
// 	c = malloc((i + 1) * sizeof(char));
// 	if (!c)
// 		return (0);
// 	i = 0;
// 	while (p[i])
// 	{
// 		c[i] = p[i];
// 		i++;
// 	}	
// 	c[i] = '\0';
// 	return (c);
// }

// char	*ft_substr(char const *s, unsigned int start, int len)
// {
// 	char	*p;
// 	int	i;

// 	if (!s)
// 		return (NULL);
// 	if ((start >= ft_strlen(s)) || len == 0)
// 		return (ft_strdup(""));
// 	if (len > ft_strlen(s) - start)
// 		len = ft_strlen(s) - start;
// 	p = (char *)malloc((len + 1) * sizeof(char ));
// 	if (!p)
// 		return (NULL);
// 	i = 0;
// 	while (i < len && s[start])
// 		p[i++] = s[start++];
// 	p[i] = '\0';
// 	return (p);
// }


t_arg	*last_arg(t_arg *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void    delete_arg(t_arg *p)
{
    t_arg   *iter;

    while (p)
    {
        iter = p;
        p = p->next;
        free(iter->arg);
        free(iter);
    }
}

int is_redirection(char c)
{
    if (c == '<' || c == '>')
        return (1);
    return (0);
}

int WildcardCheck(t_arg *r, t_arg *node)
{
    t_arg   *iter;
    int     check;

    check = 0;
    iter = r;
    if (!r)
        return (check);
    if (!node->linked)
        return (1);
    if (node->linked && !ft_strcmp(node->arg, ""))
    {
        while (iter->next != node)
            iter = iter->next;
        check = WildcardCheck(r, iter);
    }
    else if(node->linked && ft_strcmp(node->arg, ""))
        return (check);
    if (!check)
        return (check);
    return (1);
}

// t_arg	*new_arg(char *arg, char linked)

int after_star(t_star s)
{
    int i;

    i = s.index + 1;
    while (s.buf[i])
    {
        if (is_empty(s.buf + i))
            i += 2;
        else if (is_space(s.buf[i]))
            return (1);
        else
            return (0);
    }
    return (1);
}

t_arg	*new_arg(t_env **env_p, t_arg *r,t_star s, char linked)
{
	t_arg	*new;
	char	*tmp;
    char *arg;
	// char	holder;
	// int		i;

	// i = 0;
    (void)env_p;
    arg = s.arg;
	new = malloc (sizeof(t_arg));
	if (!new || !arg)
		return (new);
		// return (printf("issue mallocing\n"), new);
	new->linked = linked;
	new->redirection = s.is_rediraction;
    if (arg[0] == '\'' || is_redirection(arg[0]))
    {
        if (arg[0] == '\'')
        {
            tmp = arg;
            arg = ft_substr(arg, 1, ft_strlen(arg) - 2);
            free(tmp);
        }
    }
    else
    {
        if (arg[0] == '"')
        {
            tmp = arg;
            arg = ft_substr(arg, 1, ft_strlen(arg) - 2);
            free(tmp);
            arg = replace_env(arg, env_p);
        }
        else
        {
            if (!ft_strcmp(arg, "*") && WildcardCheck(r, last_arg(r)) && after_star(s))
            {
                arg = full_replace_env(arg, env_p);
            }
            else
            {
                arg = replace_env(arg, env_p);
            }
        }
    }
    new->arg = arg;
    new->next = NULL;
	// new->expand = expand;
	// if (type == '"' || type == 's')
	// 	new->expand = true;
	// if (type == '\'')
	// 	i = closing_quote(arg, 0);
	// else if (type == '"')
	// 	i = closing_d_quote(arg, 0);
	// else
	// 	i = end_of_arg(arg, 0);
	// holder = arg[i - 1];
	// arg[i - 1] = '\0';
	// new->arg = ft_strdup(arg);
	// arg[i - 1] = holder;
	// if (arg[i] && !is_space(arg[i]))
	// 	new->linked = true;
	return (new);
}



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

// int is_quoted(char *s, char *mark)
// {
//     char    quote;
//     int     i;

//     i = 0;
//     while (s[i])
//     {
//         if ((s[i] == '"' || s[i] == '\'') && (s + i) > mark)
//             return (0);
//         else if (s[i] == '"' || s[i] == '\'')
//         {
//             quote = s[i++];
//             while (s[i] && s[i] != quote)
//             {
//                 if ((s + i) == mark)
//                     return (1);
//                 i++;
//             }
//         }
//         i++;
//     }
//     return (0);
// }

// int quoted_space(char *s, int i)
// {
//     if (is_space(s[i]) && !is_quoted(s, s + i))
//         return (1);
//     return (0);
// }

int quoted_redirection(char *s, int i)
{
    if ((s[i] == '<' || s[i] == '>') && !is_quoted(s, s + i))
        return (1);
    return (0);
}


int end_of_arg(char *s)
{
    char    hold;
    int     i;

    i = 0;
    if (s[i] == '"' || s[i] == '\'')
    {
        hold = s[i++];
        while (s[i] && s[i] != hold)
            i++;
        return (i);
    }
    while (s[i] && s[i] != '"' && s[i] != '\'' && !is_space(s[i]) && !is_redirection(s[i]))
    // while (s[i] && !quoted_space(s, i) && !quoted_redirection(s, i))
        i++;
    if (s[i] == '"' || s[i] == '\'' || is_space(s[i]) || is_redirection(s[i]))
        i--;
    return (i);
}

int end_of_name(char *s)
{
    int i;

    i = 0;
    while (s[i] == '>' || s[i] == '<')
        i++;
    while (is_space(s[i]))
            i++;
    while (s[i] && !quoted_space(s, i) && !quoted_redirection(s, i))
        i++;
    if (s[i])
        i--;
    return (i);
}

// t_arg	*last_arg(t_arg *lst);
// t_arg	*new_arg(char *arg, char type);
// int		add_back_arg(t_arg **lst, t_arg *new);

char    *new_name(char *name, int len)
{
    char    *r;
    int     i;

    i = 0;
    while (!name[i] && i < len)
                i++;
    if (i == len)
        return (ft_strdup(""));
    r = ft_strdup(name + i);
    while (name[i] && i < len)
        i++;
    while (i < len)
    {
        if (name[i])
        {
            r = join_free(r, name + i);
            while (name[i] && i < len)
                i++;
        }
        else
            i++;
    }
    return (r);
}

char    *empty_out(char *name)
{
    // char    *r;
    int     len;
    int     i;

    i = 0;
    len = ft_strlen(name);
    while (i < len)
    {
        if (i < len && name[i] == '\'')
        {
            name[i++] = 0;
            while (name[i] && name[i] != '\'')
                i++;
            name[i++] = 0;
        }
        if (i < len && name[i] == '"')
        {
            name[i++] = 0;
            while (name[i] && name[i] != '"')
                i++;
            name[i++] = 0;
        }
        if (i < len && name[i] != '"' && name[i] != '\'')
            i++;
    }
    return (new_name(name, len));
}



char *set_up_name(char *buf, char *special_string)
{
    char    *name;
    char    *tmp;
    char    hold;
    int     i;

    i = 0;
    // (void) special_string;
    while (is_redirection(buf[i]))
                i++;
    hold = buf[i - 1];
    buf[i - 1] = 0;
    while (is_space(buf[i]))
        i++;
    name = ft_strdup(buf + i);
    tmp = name;
    name = empty_out(name);
    free(tmp);
    i = 0;
    while (buf[i])
        i++;
    buf[i++] = hold;
    buf[i] = 0;
    buf = join_free(buf, special_string);
    buf = join_free(buf, name);
    buf = join_free(buf, special_string);
    free(name);
    return (buf);
}

t_arg    *remove_qoutess(t_arg *comand)
{
    t_arg   *iter;
    // char    *name;
    char    special_string[2];

    iter = comand;
    special_string[0] = 1;
    special_string[1] = 0;
    while (iter)
    {
        if ((is_redirection(iter->arg[0]) && is_redirection(iter->arg[1]) && iter->arg[2]) || (is_redirection(iter->arg[0]) && iter->arg[1]))
        {
            // while (is_redirection(iter->arg[i]))
            //     i++;
            // iter->arg[i - 1] = 0;
            // while (is_space(iter->arg[i]))
            //     i++;
            // name = ft_strdup(iter->arg + i);
            // tmp = name;
            // name = empty_out(name);
            // free(tmp);
            // iter->arg = join_free(iter->arg, name);
            // free(name);
            // tmp = iter->arg;
            iter->arg = set_up_name(iter->arg, special_string);
            // free(tmp);
        }
        iter = iter->next;
    }
    return (comand);
}

t_arg *get_args(char *s, t_env **env)
{
    t_arg   *r;
    int     check;
    int     index;
    t_star  star;
    int i;

    i = 0;
    r = NULL;
    star.buf = s;
    while (s[i])
    {
        while (is_space(s[i]))
            i++;
        index = i;
        star.index = i;
        if (s[i] == '<' || s[i] == '>')
        {
            i += end_of_name(s + i);
            star.arg = ft_substr(s, index, i - index + 1);
            star.is_rediraction = 1;
            check = add_back_arg(&r, new_arg(env, r, star, 0));
        }
        else
        {
            i += end_of_arg(s + i);
            star.is_rediraction = 0;
            if (s[i + 1] && !is_space(s[i + 1]))
            {
                star.arg = ft_substr(s, index, i - index + 1);
                check = add_back_arg(&r, new_arg(env, r, star, 1));
            }
            else
            {
                star.arg = ft_substr(s, index, i - index + 1);
                check = add_back_arg(&r, new_arg(env, r, star, 0));
            }
        }
        if (check)
            return (delete_arg(r), NULL);
        if (!s[i])
            break ;
            // return (NULL, printf("issue mallocing\n"));
        i++;
    }
    r = remove_qoutess(r);
    return (r);
}
// int first_redirection(char *s)
// {
//     int i;

//     i = 0;
//     while (s[i])
//     {
// 		if (s[i] == 1)
// 			i += ignore_alpha(s + i, 1);
//         if ((s[i] == '<' || s[i] == '>') && !is_quoted(s, s + i))
//             return (i);
//         i++;
//     }
//     return (-1);
// }


// int main()
// {
//     char *str;

//     str = "\0";
//     t_arg   *r = get_args("cat '<' cat < sa'''lam << mom'kin ta3tini chi 7ota");
//     // t_arg *p = r;
//     // t_arg *k = r;

//     // while (k)
//     // {
//     //     while (p->linked)
//     //     {
//     //         p->arg = ft_strjoin()
//     //     }
//     // }
//     r = remove_qoutes(r);
//     while (r)
//     {
//         printf("|%s|=|%d|\n", r->arg, r->linked);
//         str = ft_strjoin(str, r->arg);
//         r = r->next;
//     }
//     printf("%s\n", str);
//     printf("%d\n", first_redirection(str));
// }