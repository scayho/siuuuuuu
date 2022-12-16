#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "minishell.h"

// cat <<""'    '""ko<<""''"'" < zebzabi

// int	ft_isalpha(int c)
// {
// 	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
// 		return (1);
// 	return (0);
// }

// int	ft_isdigit(int c)
// {
// 	if (c >= '0' && c <= '9')
// 		return (1);
// 	return (0);
// }

// int	ft_isalnum(int c)
// {
// 	if (ft_isdigit(c) || ft_isalpha(c))
// 		return (1);
// 	return (0);
// }


// int	is_space(char c)
// {
// 	if (c == '\t' || c == '\n' || c == ' ' || c == '\v'
// 		|| c == '\f' || c == '\r')
// 		return (1);
// 	return (0);
// }

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

// int first_redirection(char *s)
// {
//     int i;

//     i = 0;
//     while (s[i])
//     {
//         if ((s[i] == '<' || s[i] == '>') && !is_quoted(s, s + i))
//             return (i);
//         i++;
//     }
//     return (-1);
// }


// int is_quote(char c)
// {
//     if (c == '\'' || c == '"')
//         return (1);
//     return (0);
// }

// int	is_there_file(char *s)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i] && is_space(s[i]))
// 		i++;
// 	if (!s[i])
// 		return (0);
// 	if (s[i] && (ft_isalnum(s[i]) || s[i] == '/' || is_quote(s[i])))
// 		return (1);
// 	return (0);
// }

int first_pipe(char *s)
{
    int i;

    i = 0;
    // printf("'%s'\n", s);
    while (s[i])
    {
        if (s[i] == '|' && !is_quoted(s, s + i))
            return (i);
        i++;
    }
    return (-1);
}

int pipe_first(char *buf)
{
    int i;

    i = 0;
    while (buf[i] && is_space(buf[i]))
        i++;
    if (buf[i] == '|')
        return (1);
    return (0);
}

int pipe_syntax(char *buf)
{
    int i;

    i = 0;
    if (pipe_first(buf))
        return (1);
    while (first_pipe(buf + i) != -1)
    {
        i += first_pipe(buf + i) + 1;
        // printf("{%d}\n", i);
        while (buf[i] && is_space(buf[i]))
            i++;
        if ((buf[i] == '<' && buf[i + 1] == '<') ||
            (buf[i] == '>' && buf[i + 1] == '>'))
            i += 2;
        else if (buf[i] == '>')
            i++;
        else if (buf[i] == '<')
            return (1);
        // i++;
        // printf("[%s]\n", buf + i);
        if (!is_there_file(buf + i))
            return (1);
    }
    return (0);
}

// int is_redirection(char c)
// {
//     if (c == '>' || c == '<')
//         return (1);
//     return (0);
// }

int redirection_syntax(char *buf)
{
    int i;

    i = 0;
    while (first_redirection(buf + i) != -1)
    {
        i += first_redirection(buf + i);
        // printf("{%s}\n", buf + i);
        if ((buf[i] == '>' && buf[i + 1] == '>') || (buf[i] == '<' && buf[i + 1] == '<'))
        {
            if (!is_there_file(buf + i + 2))
                return (1);
            i += 2;
        }
        else if (buf[i] == '>' || buf[i] == '<')
        {
            if (!is_there_file(buf + i + 1))
                return (1);
            i++;
        }
        // else if (buf[i] == '<' && buf[i + 1] == '<')
        // {
        //      if (!is_there_file(buf + i + 2))
        //         return (1);
        //     i += 2;
        // }
        // else if (buf[i] == '<')
        // {
        //     if (!is_there_file(buf + i + 1))
        //         return (1);
        //     i++;
        // }
    }
    return (0);
}

// int main()
// {
//     printf("%d\n", pipe_syntax("echo | << hi | cat sama"));
//     // printf("%d\n", redirection_syntax("echo< | << '<<'  hi | cat sama "));

// }