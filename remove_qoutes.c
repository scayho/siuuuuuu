#include "minishell.h"

int    markqout(char    *buf)
{
    int len;
    int i;

    i = 0;
    len = ft_strlen(buf);
    while (i < len)
    {
        if (buf[i] == '\'')
        {
                buf[i] = '\0';
                    while(buf[i] != '\'' && i < len)
                    {
                        i++;
                    }
                buf[i] = '\0';
        }
        else if (buf[i] == '"')
        {
                buf[i] = '\0';
                while(buf[i] != '"' && i < len)
                {
                    i++;
                }
                buf[i] = '\0';
        }
        else
            i++;
    }
    return (len);
}

char    *eraseqout(char *buf, int len)
{
    int i;
    char *tmp;
    char *tmp2;
    char *tmp1;

    i = 0;
    // tmp1 = malloc(1);
    // if (!tmp1)
    // {
    //     printf("wa zabi wa wa wa \n");
    //     exit(0);
    // }
    // printf("&&&&&&&&&&&&& :%s\n", buf);
    tmp1 = "\0";
    tmp = NULL;
    while (buf[i] == '\0' && i < len)
        i++;
    tmp = ft_strdup(buf + i);
    tmp1 = ft_strjoin(tmp1, tmp);
    free(tmp);
    while (buf[i] != '\0' && i < len)
        i++;
    // printf("rah l i f : %d o len f %d : %s\n", i, len, buf);
    while (i < len)
    {
        // printf("rah l i f : %d o len f %d : %s\n", i, len, buf);
        while (buf[i] == '\0' && i < len)
            i++;
        tmp = ft_strdup(buf + i);
        tmp2 = tmp1;
        tmp1 = ft_strjoin(tmp1, tmp);
        free(tmp);
        free(tmp2);
        while (buf[i] != '\0' && i < len)
            i++;
    }
    return (tmp1);
}
t_arg    *remove_qoutes(t_arg				*command)
{
    t_arg				*cmd;
    int len;
    char    *tmp;
    char    *del;
    char    specel_charachter[2];
    int     i;

    cmd = command;
    len = 0;
    i = 0;
    specel_charachter[0] = 1;
    specel_charachter[1] = '\0';
    while(command)
    {
        if (command->arg[0] == '>' || command->arg[0] == '<')
        {
            if (command->arg[1] == '>' || command->arg[1] == '<')
            {
                del = ft_strdup(command->arg + 2);
                command->arg[2] = '\0';
            }
            else
            {
                del = ft_strdup(command->arg + 1);
                command->arg[1] = '\0';
            }
            while(is_space(del[i]))
                i++;
            len = markqout(del + i);
            // printf("wwwwwwww :[%s]\n", del + i);
            tmp = del;
            del = eraseqout(del + i, len);
            free(tmp);
            tmp = del;
            del = ft_strjoin(specel_charachter, del);
            free(tmp);
            tmp = del;
            del = ft_strjoin(del, specel_charachter);
            free(tmp);
            tmp = command->arg;
            command->arg = ft_strjoin(command->arg, del);
            free(tmp);
            free(del);
        }
        // printf("lakhra : %s\n", command->arg);
        command = command->next;
    }
    // free(specel_charachter);
    return (cmd);
}
// char    *remove_qoutes(char *buf)
// {
//     int len;
//     char    *tmp;

//     len = 0;
//     len = markqout(buf);
//     // write(1, tmp, len);
//     // printf("]\n%d\n", len);
//     // exit(0);
//     tmp = buf;
//     buf = eraseqout(buf, len);
//     free(tmp);
//     return (buf);
// }