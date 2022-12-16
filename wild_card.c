#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "minishell.h"

// int    ft_strlen(const char *s)
// {
//     int    i;

//     i = 0;
//     if (!s)
//         return (0);
//     while (s[i])
//         i++;
//     return (i);
// }

char    *ft_strjoin(char const *s1, char const *s2)
{
    char    *r;
    int        i;
    int        j;

    if (!s1 || !s2)
        return (0);
    i = ft_strlen(s1);
    j = ft_strlen(s2);
    r = malloc ((i + j + 1) * sizeof(char));
    if (!r)
        return (NULL);
    i = 0;
    while (s1[i])
    {
        r[i] = s1[i];
        i++;
    }
    j = 0;
    while (s2[j])
        r[i++] = s2[j++];
    r[i] = '\0';
    return (r);
}

// char    *join_free(char const *s1, char const *s2)
// {
//     char    *r;

//     r = ft_strjoin(s1, s2);
//     if (!r)
//         return (NULL);
//     free((char *)s1);
//     return (r);
// }
char    *wildcard()
{
    struct dirent   *file_dir;
    char    *r;
    DIR *dir;

    r = NULL;
    dir = opendir(".");
    if (!dir)
    {
        write(2, "Opendir issue\n", 14);
        return (NULL);
    }
    file_dir = readdir(dir);
    if (!file_dir)
        return (NULL);
    if (file_dir->d_name[0] != '.')
        r = ft_strjoin(file_dir->d_name, " ");
    else
        r = ft_strdup("");
    while (file_dir)
    {
        if (file_dir->d_name[0] != '.')
        {
            r  = join_free(r, file_dir->d_name);
            r = join_free(r, " ");
        }
        file_dir = readdir(dir);
    }
    closedir(dir);
    return (r);
}

// int main()
// {
//     printf("%s\n", wildcard());
//     system("leaks a.out");
// }