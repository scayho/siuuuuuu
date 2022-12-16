#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"


char	*ft_strdup(const char *s1)
{
	int		i;
	char	*c;
	char	*p;

	i = 0;
	p = (char *)s1;
	if (!s1)
		return (0);
	while (s1[i])
		i++;
	c = malloc((i + 1) * sizeof(char));
	if (!c)
		return (0);
	i = 0;
	while (p[i])
	{
		c[i] = p[i];
		i++;
	}	
	c[i] = '\0';
	return (c);
}
// char	*exlude_name(char	*str, int start, int end)
// {
// 	char	*tmp;
// 	char	*tmp1;

// 	// printf("////%s|\n", str + start);
// 	str[start++] = '\0';
// 	// if (ft_strcmp(str, ">>") || ft_strcmp(str, "<<"))
// 	if (str[start] == '>' || str[start] == '<')
// 		start++;
// 	while (is_space(str[start]))
// 		start++;
// 	tmp = ft_strdup(str);
// 	tmp1 = ft_strdup(str + start + end + 1);
// 	// printf("zzzzz:{%s}{%d}{%d}\n", str, start, end);
// 	free(str);
// 	str = join_free(tmp, " ");
// 	str = join_free(str, tmp1);
// 	free(tmp1);
// 	return (str);
// }

// int	is_space(char c)
// {
// 	if (c == '\t' || c == '\n' || c == ' ' || c == '\v'
// 		|| c == '\f' || c == '\r')
// 		return (1);
// 	return (0);
// }

int is_quoted(char *s, char *mark)
{
    char    quote;
    int     i;

    i = 0;
    while (s[i])
    {
        if ((s[i] == '"' || s[i] == '\'') && (s + i) > mark)
            return (0);
        else if (s[i] == '"' || s[i] == '\'')
        {
            quote = s[i++];
            while (s[i] && s[i] != quote)
            {
                if ((s + i) == mark)
                    return (1);
                i++;
            }
        }
        i++;
    }
    return (0);
}

int	isThereRedirection(t_arg *s)
{
	t_arg	*iter;

	iter = s;
	while(iter)
	{
		if(iter->redirection)
			return (1);
		iter = iter->next;
	}
	return (0);
}

int first_redirection(char *s)
{
    int i;

    i = 0;
    while (s[i])
    {
		if (s[i] == 1)
			i += ignore_alpha(s + i, 1);
        if ((s[i] == '<' || s[i] == '>') && !is_quoted(s, s + i))
            return (i);
        i++;
    }
    return (-1);
}

int redirection_count(char *s)
{
    int count;
    int i;

    i = 0;
    count = 0;
    while (first_redirection(s + i) != -1)
	{
		i += first_redirection(s + i);
		while (s[i] && (s[i] == '<' || s[i] == '>'))
        	i++;
        count++;
	}
    return (count);
}

int	skip_to_name(char *s)
{
	char	mark;
	int	i;

	i = first_redirection(s);
	while (s[i] && (s[i] == '<' || s[i] == '>'))
        i++;
	while (s[i] && is_space(s[i]))
        i++;
	// while (s[i] && !is_space(s[i]) &&  !(s[i] == '<' || s[i] == '>'))
	if (s[i] == '"' || s[i] == '\'')
	{
		mark = s[i];
		i++;
		while (s[i] != mark)
			i++;
		i++;
	}
	while (s[i] && !(s[i] == '<' || s[i] == '>'))
        i++;
	return (i);
}

char    **redirection_split(char *s)
{
    char    **r;
    int     i;
    int     j;
	int		k;

    i = 0;
    j = 0;
	k = 0;
    r = malloc ((redirection_count(s) + 1) * sizeof(char *));
    if (!r)
        return (NULL);
	while (first_redirection(s + i) != -1)
	{
		k = skip_to_name(s + i);
		r[j++] = ft_substr(s, i, k);
		i += k;
	}
    r[j] = NULL;
    return (r);
}

// int main()
// {
//     char **r = redirection_split("echo hi << \"<<\" are you >> suup>>ayo<< \"s allam\" <cat hi");
// 	int	i = 0;
// 	while (r[i])
//     	printf("%s\n", r[i++]);
// }

char	*redirection_name(char *buf)
{
	int i;
	char	*name;
	int 	start;

	i = 0;
	// while(buf[i])
	// {
	// 	if (buf[i] == 1)
	// 		printf("hjgghjjgh\n");
	// 	else
	// 		printf("[%c]\n", buf[i]);
	// 	i++;
	// }
	// i = 0;
	while(buf[i] != 1 && buf[i])
		i++;
	if (buf[i] == 1)
		start = ++i;
	while(buf[i] != 1 && buf[i])
		i++;
	if (buf[i] == 1)
	{
		buf[i] = '\0';
	}
	name = ft_strdup(buf + start);
	// printf("[[[[%s]]]]\n", name);
	return (name);
}

// char	*redirection_name(char *buf)
// {
// 	char	*name;
// 	// char	hold;
// 	int		start;
// 	int	i;

// 	i = 0;
// 	start = 0;
// 	while (buf[i] && is_space(buf[i]))
// 	{
// 		start++;
// 		i++;
// 	}
// 	if (is_empty(buf + start) && (is_space(buf[start + 2]) || buf[start + 2] != '<' || buf[start + 2] != '>'))
// 		return ("");
// 	else if (buf[i] == '"' || buf[i]  == '\'')
// 	{
// 		start++;
// 		if (buf[i] == '"')
// 			i += ignore_alpha(buf + i, '"');
// 		else
// 			i += ignore_alpha(buf + i, '\'');
// 	}
// 	else
// 	{
// 		while(!is_space(buf[i]) && buf[i] != '<' && buf[i] != '>' && buf[i])
// 			i++;
// 	}
// 	// if (buf[i] == '"' || buf[i]  == '\'')
// 	// {
// 	// 	hold = buf[i++];
// 	// 	while (buf[i] != hold)
// 	// 		i++;
// 	// 	buf[i] = 0;
// 	// 	name = ft_strdup(buf + 1);
// 	// 	return (name);
// 	// }
// 	// start = i;
// 	// while (buf[i])
// 	// {
// 	// 	if (buf[i] == ' ' || buf[i] == '<' || buf[i] == '>')
// 	// 		break;
// 	// 	i++;
// 	// }
// 	buf[i] = '\0';
// 	name = ft_strdup(buf + start);
// 	free(buf);
// 	return (name);
// }

// char	*exlude_name(char	*str, int start, char *name)
// {
// 	char	*tmp;
// 	char	*tmp1;

// 	str[start] = '\0';
// 	start++;
// 	while(ft_strncmp(str + start, name, ft_strlen(name)) != 0)
// 		start++;
// 	tmp = ft_strdup(str);
// 	tmp1 = ft_strdup(str + start + ft_strlen(name));
// 	free(str);
// 	str = join_free(tmp, " ");
// 	str = join_free(str, tmp1);
// 	free(tmp1);
// 	return (str);
// }
// char	*exlude_name(char	*str, int start, int end)
// {
// 	char	*tmp;
// 	char	*tmp1;

// 	str[start] = '\0';
// 	start++;
// 	// if (ft_strcmp(str, ">>") || ft_strcmp(str, "<<"))
// 	if (str[start] == '>' || str[start] == '<')
// 		start++;
// 	while (is_space(str[start]))
// 		start++;
// 	tmp = ft_strdup(str);
// 	if (end == -1)
// 		tmp1 = ft_strdup(str + start + 2);
// 	else
// 		tmp1 = ft_strdup(str + start + end);
// 	free(str);
// 	str = join_free(tmp, " ");
// 	str = join_free(str, tmp1);
// 	free(tmp1);
// 	return (str);
// }
char	*exlude_name(char	*str, int start, int end)
{
	char	*new;
	int i;

	i = 0;
	new = NULL;
	if (end == 2)
	{
		str[start] = '\0';
		start++;
		while (str[start] != 1 && str[start])
			start++;
		start++;
		while (str[start] != 1 && str[start])
			start++;
		new = ft_strdup(str);
		new = ft_strjoin(new, str + start + 1);
	}
	else if (end == 0)
	{
		str[start] = '\0';
		start++;
		i = start;
		while (str[start] != 1 && str[start])
			start++;
		if (str[start] == 1)
			str[start] = ' ';
		while (str[start] != 1 && str[start])
			start++;
		if (str[start] == 1)
			str[start] = ' ';
		new = ft_strdup(str);
		new = ft_strjoin(new, " ");
		new = ft_strjoin(new, str + i);
	}
	free(str);
	return (new);
}

int first_heredoc(char *s, int hd)
{
    int i;

    i = 0;
    while (s[i])
    {
		if (s[i] == 1)
			i += ignore_alpha(s + i, 1);
        if ((s[i] == '<' && s[i + 1] == '<') && !is_quoted(s, s + i))
            return (i);
        else if ((s[i] == '<' && s[i + 1] != '<' && s[i - 1] != '<') && !is_quoted(s, s + i) && hd == 2)
            return (i);
        i++;
    }
    return (-1);
}


int outputCheck(char *s, int hd)
{
    int i;

    i = 0;
    while (s[i])
    {
		if (s[i] == 1)
			i += ignore_alpha(s + i, 1);
        if ((s[i] == '>' && s[i + 1] == '>') && !is_quoted(s, s + i))
            return (i);
        else if ((s[i] == '>' && s[i + 1] != '>' && s[i - 1] != '>') && !is_quoted(s, s + i) && hd == 2)
            return (i);
        i++;
    }
    return (-1);
}

int first_input(char *s)
{
    int i;

    i = 0;
    while (s[i])
    {
        if ((s[i] == '<' && s[i + 1] != '<' && s[i - 1] != '<') && !is_quoted(s, s + i))
            return (i);
        i++;
		printf("[%s]<<<<<<%d\n", s, i);
    }
    return (-1);
}


// char	*exlude_name(char	*str, int start, int end)
// {
// 	char	*tmp;
// 	char	*tmp1;
// 	int     bb;

// 	bb = 0;
// 	str[start] = '\0';
// 	start++;
// 	// if (ft_strcmp(str, ">>") || ft_strcmp(str, "<<"))
// 	if (str[start] == '>' || str[start] == '<')
// 		start++;
// 	while (is_space(str[start]))
// 		start++;
// 	while (str[start] == '"' || str[start] == '\'')
// 	{
// 		bb++;
// 		start++;
// 	}
// 	tmp = ft_strdup(str);
// 	if (end == -1)
// 		tmp1 = ft_strdup(str + start + 2);
// 	else if (bb == 1)
// 		tmp1 = ft_strdup(str + start + end + 1);
// 	else
// 		tmp1 = ft_strdup(str + start + end);
// 	free(str);
// 	// str = join_free(tmp, " ");
// 	str = join_free(tmp, tmp1);
// 	free(tmp1);
// 	return (str);
// }
// char	*redirection_name(char *buf)
// {
// 	char	*name;
// 	// char	hold;
// 	int		start;
// 	int	i;

// 	i = 0;
// 	start = 0;
// 	while (buf[i] && is_space(buf[i]))
// 	{
// 		start++;
// 		i++;
// 	}
// 	if (is_empty(buf + start) && (is_space(buf[start + 2]) || buf[start + 2] != '<' || buf[start + 2] != '>'))
// 		return ("");
// 	else if (buf[i] == '"' || buf[i]  == '\'')
// 	{
// 		start++;
// 		if (buf[i] == '"')
// 			i += ignore_alpha(buf + i, '"');
// 		else
// 			i += ignore_alpha(buf + i, '\'');
// 		i--;
// 	}
// 	else
// 	{
// 		while(!is_space(buf[i]) && buf[i] != '<' && buf[i] != '>' && buf[i])
// 			i++;
// 	}
// 	buf[i] = '\0';
// 	printf("name is [%s]\n", buf + start);
// 	name = ft_strdup(buf + start);
// 	free(buf);
// 	return (name);
// }