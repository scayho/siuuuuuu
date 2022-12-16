#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
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
int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	ft_isalnum(int c)
{
	if (ft_isdigit(c) || ft_isalpha(c))
		return (1);
	return (0);
}

int is_quote(char c)
{
    if (c == '"' || c == '\'')
        return (1);
    return (0);
}

int still_in_name(char c)
{
    if (ft_isalnum(c) || c == '_')
        return (1);
    return (0);
}

int	closing_quote(char *line, char quote, int i)
{
	while (line[i] != quote)
		i++;
	i++;
	return (i);
}



int is_squoted(char *s, char *mark)
{
    int     i;

    i = 0;
    while (s[i])
    {
        if (s[i] == '\'' && (s + i) > mark)
            return (0);
        else if (s[i] == '\'')
        {
            i++;
            while (s[i] && s[i] != '\'')
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

int quoted_space(char *s, int i)
{
    // printf(">> [%c] [%d] [%d]\n", s[i], is_space(s[i]), is_quoted(s, s + i));
    if (is_space(s[i]) && !is_quoted(s, s + i))
        return (1);
    return (0);
}

int quoted_heredoc(char *s, int i)
{
    if (s[i] == '<' && !is_quoted(s, s + i))
        return (1);
    return (0);
}

int heredoc_name(char *buf, char *mark)
{
    // char    hold;
    int     i;

    i = 0;
    while (buf[i])
    {
        if (buf[i] == '<' && buf[i + 1] == '<' && !is_quoted(buf, buf + i) && buf + i <= mark)
        {
            i += 2;
            while (is_space(buf[i]))
                i++;
            if (buf + i > mark)
                return (0);
            while (buf[i] && !quoted_space(buf, i) && !quoted_heredoc(buf, i))
            {
                // printf(" [%c] [%d] [%d] [%d]  \n", buf[i], i , quoted_space(buf, i), quoted_heredoc(buf, i));
                if(buf + i == mark)
                   return (1);
                i++;
            }
        }
        i++;
    }
    return (0);
}

char    *expander(char *buf, t_env **env)
{
    char    *tmp;
    char    *hold;
    int     index;
    int     i;

    i = 0;
    while (buf[i])
    {
        if (buf[i] == '$' && !is_squoted(buf, &buf[i]) && !heredoc_name(buf, &buf[i]))
        {
            i++;
            index = i;
			while (buf[i] && still_in_name(buf[i]))
				i++;
			tmp = ft_substr(buf, index, i - index);
			if  (env_exist(env, tmp))
			{
				hold = buf;
				buf = expand_it(buf, tmp, index, i - 1, env);
                i = index + ft_strlen(extract_evar_value(*env, tmp)) - 2;
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
        }
        i++;
    }
    return (buf);
}

// during expanding hold where the quotes start and end send to function to remove them and continue cycling the string


// int main()
// {
//     // char *s = "<<$PWD $USER wsuuujp";
//     char *s = "echo hi <<\"$\"ko\" \" <<hi <<$PWD $USER wsuuujp";

//     // int i = 0;
//     // while(s[i])
//     // {
//     //     printf("[%c]  [%d]\n", s[i], i);
//     //     i++;
//     // }
//     // printf("%d\n", expander("echo $PWD$HOME '$PWD' hi <<\"$\"<<hi <<$PWD wsuuup"));
//     printf("%s\n%d\n", s + 15, heredoc_name(s, s + 7));

// remove all qoiutes 