
#include "minishell.h"

int are_qouted(char *str, char  *target)
{
    int i;

    i = 0;
    while(str[i])
    {
        if (str[i] == '\'' || str[i] == '"')
        {
            if (str[i] == '\'')
                i += ignore_alpha(str + i, '\'');
            else if (str[i] == '"')
                i += ignore_alpha(str + i, '"');
            if (str + i > target)
            {
                // printf("raha qouted\n");
                return (1);
            }
        }
        else
            if (str + i == target)
                return (0);
        i++;
    }
    return (0);
}

int     last_red_in(char *or, char    *str)
{
    int i;
    // int j;

    // j = 0;
    i = 0;
    while (str[i] == '<')
        i++;
    (void)or;
    // while (str[j])
    // {
    //     if (str[j] == 1)
    //         printf("zebi    ");
    //     else
    //         printf("[%c]    ", str[j]);
    //     j++;
    // }
    // printf("\nstring to check next red in :%s\n", str);
    while(str[i] != '\0')
    {
        if (str[i] == 1)
            i += ignore_alpha(str + i, 1);
        // printf("-[%c] [%c] [%c] [%c] [%d] [%d]\n", str[i - 1], str[i], str[i + 1], str[i + 2], i, are_qouted(or, str + i));
        // printf("wa zabi %s , %d , %d\n", str + i, heredoc_name(str, str + i), is_quoted(or, str + i));
        // printf("ééééé :%s\n", str + i);
        if (((str[i] == '<' && str[i + 1] == '<' && str[i + 2] != '<') 
        || (str[i] == '<' && str[i + 1] != '<' && str[i - 1] != '<')))
        // && are_qouted(or, str + i) == 0)
        {
            // printf("[%c] [%c] [%c] [%c] [%d] [%d]\n", str[i - 1], str[i], str[i + 1], str[i + 2], i, are_qouted(or, str + i));
            // printf("khrjat men %d \n", i);
            return (1);
        }
        i++;
    }
    return (0);
}
        // && !are_qouted(or, str + i))) && !heredoc_name(or, str + i))

// int main(void)
// {
//     char *str = ft_strdup("<<\"\"<<\"\"");


//     // while(str[i])
//     // {
//     //     printf("[%c] [%d]\n", str[i], i);
//     //     i++;
//     // }
//     printf("\n%d\n", last_red_in(str, str));
//     return (0);
// }