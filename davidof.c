#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "minishell.h"


#include <termios.h>
#include <errno.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}
char *clear_extra_qoutes(char   *buf)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (buf[i] != '\0')
    {
        if (buf[i] == '\'')
        {
            while (buf[i + j] == '\'' && buf[i + j] != '\0')
                j++;
            if (!is_space(buf[i + j]) && buf[i + j] != '\0')
            {
                buf = exlude_name(buf, i, j);
                printf("end the exlude : [%s] \nend at [%s]\n", buf + i, buf + i + j);
            }
            else
                i += j;
            j = 0;
        }
        else if (buf[i] == '\"')
        {
            while (buf[i + j] == '\"' && buf[i + j] != '\0')
                j++;
            if (!is_space(buf[i + j]) && buf[i + j] != '\0')
            {
                buf = exlude_name(buf, i, j);
            }
            else
                i += j;
            j = 0;
        }
        else
            i++;
    }
    return (buf);
}

int is_empty(char   *str)
{
    int i;

    i = 0;
    // printf("> %s\n", str);
    if (str[i] == '"' && str[i + 1] == '"')
        return (1);
    else if (str[i] == '\'' && str[i + 1] == '\'')
        return (1);
    return(0);
}

int hereco_before(char  *origin, char    *target)
{
    int i;

    i = 0;
    while(target - i > origin)
    {
        while (!is_space(target[i]) && target - i > origin)
        {
            if (target[i] == '<' || target[i] == '>')
            {
                return (1);
            }
            i++;
        }
        while (is_space(target[i]) && target - i > origin + 1)
        {
            i++;
        }
        if ((target[i] == '<' && target[i - 1] == '<') || target - i - origin < 2)
        {
            return (1);
        }
        else
            return (0);
    }
    return (0);
}
int	ignore_alpha(char *str, int dj)
{
	int i;

	i = 1;
	while (str[i] != dj && str[i] != '\0')
		i++;
	return (i + 1);
}

char    *exclude_empty(char *buf, int i)
{
    char *tmp;
    char *tmp1;

    tmp = NULL;
    tmp1 = NULL;
    buf[i] = '\0';
    tmp = ft_strdup(buf);
    tmp1 = ft_strdup(buf + i + 2);
    free(buf);
    buf = ft_strjoin(tmp, tmp1);
    free(tmp1);
    free(tmp);
    return (buf);
}

char *empty_string(char *buf)
{
    int i;
    int j;
    int check;

    j = 0;
    i = 0;
    check = 0;
    if (is_empty(buf + i))
    {
        printf("salam %d\n", j++);
        check = 1;
        i += 2;
    }
    while (buf[i])
    {
        if (is_empty(buf + i))
        {
            if (hereco_before(buf, buf + i - 1))
                i += 2;
            else if(!is_space(buf[i - 1]) && !is_space(buf[i + 2]))
                i += 2;
            else if (buf[i + 2] && !is_space(buf[i + 2]) && (buf[i] != '"' && buf[i] != '\''))
            {
                buf = exclude_empty(buf, i);
            }
            else
            {
                buf = exclude_empty(buf, i);
            }

        }
        else if (buf[i] == '"' || buf[i] == '\"')
        {
            if (buf[i] == '"')
                i += ignore_alpha(buf + i, '"');
            else if (buf[i] == '\'')
                i += ignore_alpha(buf + i, '\'');
        }
        else
            i++;
    }
    if (check == 1)
        buf = exclude_empty(buf, 0);
    return(buf);
}
// char    *exlude_epmty_string(char   *buf, int start)
// {
//     char    *tmp;

//     buf[start] = '\0';
//     tmp = ft_strdup(buf);
//     buf = ft_strjoin(tmp, buf + start + 2);
//     return (buf);
// }

// char *empty_string(char *buf)
// {
//     int i;
//     // int check;

//     i = 0;
//     // check = 0;
//     if (is_empty(buf + i))
//         buf = exlude_epmty_string(ft_strdup(buf), i);
//         // check = 1;
//     // i++;
//     while (buf[i])
//     {
//         if (is_empty(buf + i))
//         {
//             if (hereco_before(buf, buf + i - 1) && (is_space(buf[i + 2]) || buf[i + 2] == '<' || buf[i + 2] == '>' || buf[i + 2] == '\0'))
//             {
//                 i += 2;
//             }
//             else if(!is_space(buf[i - 1]) && !is_space(buf[i + 2]) && (buf[i] != '"' && buf[i] != '\''))
//                 i += 2;
//             else if (buf[i + 2] && !is_space(buf[i + 2]))
//                 buf = exlude_epmty_string(ft_strdup(buf), i);
//             else
//                 buf = exlude_epmty_string(ft_strdup(buf), i);
//         }
//         else if (buf[i] == '"' || buf[i] == '\"')
//         {
//             if (buf[i] == '"')
//                 i += ignore_alpha(buf + i, '"');
//             else if (buf[i] == '\'')
//                 i += ignore_alpha(buf + i, '\'');
//         }
//         else
//             i++;
//     }
//     // if (check == 1)
//     return(buf);
// }

// ## exlude only empty strings
// ''   ''  << '' '' ''ok''  ''  << '' akhi

// int main(void)
// {
//     char *s;

//     // s = ft_strdup("'' '' << '' '' ''ok'' '' << '' akhi");
//     s = ft_strdup("<< '' ''''\"''      \"'''\"\"ok'''   \"''<< \"'' <<'       <<'akhi");
// << '' ''''"''      "'''""ok'''   "''<< "'' <<'       <<'akhi
//     printf("%s\n\n\n\n", empty_string(s));

// }
// int hereco_before(char  *buf, char    *mark)
// {
//     int i;
//     char    hold;

//     i = 0;
//     while (buf[i])
//     {
//         if (buf[i] == '<' && buf[i + 1] == '<')
//         {
//             i += 2;
//             while (is_space(buf[i]))
//                 i++;
//             if (is_quote(buf[i]))
//             {
//                 hold = buf[i];
//                 if(buf + i == mark)
//                     return (1);
//                 i++;
//                 while (buf[i] != hold && buf[i])
//                 {
//                     if(buf + i == mark)
//                         return (1);
//                     i++;
//                 }
//                 if ()
//                 while (buf[i] && !hold)
//                 {
//                     if(buf + i == mark)
//                         return (1);
//                     i++;
//                 }
//                 if(buf + i == mark)
//                     return (1);
//             }
//             else
//             {
//                 while (!is_space(buf[i]))
//                 {
//                     if(buf + i == mark)
//                         return (1);
//                     i++;
//                 }
//             }
//         }
//         i++;
//     }
//     return (0);
// }
    // printf("\n\n%s\n\n", exlude_name(ft_strdup("salam cava ara chi 7awya"), 4, 8));
    //exit(0);
    // s = ft_strdup("<< \"\"        ok   << ''akhi");
    // printf("%s\n\n\n\n", empty_string(s));
    // s = ft_strdup("<< ok   << akhi");
    // printf("%s\n\n\n\n", empty_string(s));
    // s = ft_strdup("<<     \"\"   \"\"\"\"ok \"\"  << ''akhi");
    // printf("%s\n\n\n\n", empty_string(s));
    // s = ft_strdup("<< ''        ok   << ''  akhi");
    // printf("%s\n\n\n\n", empty_string(s));
    // s = ft_strdup("<<   ''     ''ok''   << ''akhi");
    // printf("%s\n\n\n\n", empty_string(s));
