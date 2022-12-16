
#include "minishell.h"

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

int n_args(char *f, int len)
{
    int n;
    int i;

    i = 0;
    n = 0;
    while (i < len)
    {
        if(f[i])
        {
            n++;
            while (f[i])
                i++;
        }
        i++;
    }
	// printf("s = |%s| n = --%d--\n",f,  n);
    return (n);
}

char    *stage_1(char *s, char c)
{
    char    *f;
    int     i;

    i = 0;
    f = ft_strdup(s);
    while (s[i])
    {
        if (s[i] == c  && !is_quoted(s, &s[i]))
		{
			// printf(">%s<\n", &s[i]);
            f[i] = '\0';
		}
        i++;
    }
			// printf(">>>%s<<<\n", s);
    return (f);
}

// void    free_dp(char **s)
// {
//     int i;

//     i = 0;
//     while (s[i])
//         free(s[i++]);
//     free(s);
// }

char    **fstage(char **r, int len, char *p, int slen)
{
    int i;
    int j;

    i = 0;
    j = -1;
    r = malloc ((len + 1) * sizeof (char *));
    if (!r)
        return (NULL);
    while (++j < slen)
    {
        if(p[j])
        {
            r[i] = ft_strdup(p + j);
			// printf("halaloya s = |%s|\n", p + j);
            if (!r[i++])
            {
                free_dp(r);
                return (NULL);
            }
            while (p[j])
                j++;
        }
		// j++;
    }
    free(p);
    r[i] = NULL;
    return (r);
}

char    **custom_split(char *s, char c)
{
    char    **r;
    char    *p;
    int     len;

	r = NULL;
    p = stage_1(s, c);
    len = n_args(p, ft_strlen(s));
    r = fstage(r, len, p, ft_strlen(s));
    return (r);
}

// custom split khasha tspliti b les qoutes/Dqoutes 
// ila kan l qoute/dqoute dakhel fl herdoc name khalih



// int main()
// {
//     char **r;
//     int i = 0;

//     r = custom_split("   echo   \"eyooo guys how are you doing\"    arigato  '''  ssuup ''", ' ');
//     if (!r)
//         printf("l3fo\n");
//     while (r[i])
//         printf("{%s}\n", r[i++]);
//     free_dp(r);
// }