/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_without_quotes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchahid <hchahid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 13:36:00 by hchahid           #+#    #+#             */
/*   Updated: 2022/11/02 02:34:49 by hchahid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	is_space(char c)
// {
// 	if (c == '\t' || c == '\n' || c == ' ' || c == '\v'
// 		|| c == '\f' || c == '\r')
// 		return (1);
// 	return (0);
// }

int	closing_d_quote(char *line, int i)
{
	while (line[i] != '"')
		i++;
	i++;
	return (i);
}

int	closing_quote(char *line, int i)
{
	while (line[i] != '\'')
		i++;
	i++;
	return (i);
}

int	end_of_arg(char *line, int i)
{
	while (line[i] && !is_space(line[i]))
		i++;
	return (i);
}

int	count_args(char *line)
{
	int		args;
	int		i;

	i = 0;
	args = 0;
	while (line[i])
	{
		while (line[i] && is_space(line[i]))
			i++;
		if (line[i] == '"')
		{
			i = closing_d_quote(line, i + 1);
			args++;
		}
		if (line[i] == '\'')
		{
			i = closing_quote(line, i + 1);
			args++;
		}
		if (line[i] && !is_space(line[i]) && line[i] != '"' && line[i] != '\'')
		{
			i = end_of_arg(line, i);
			args++;
		}
	}
	return (args);
}


char **split_without_quotes(char *line)
{
	char	**ret;
	int		args;
	int		i;
	int		j;

	i = 0;
	j = 0;
	ret = NULL;
	args = count_args(line);
	printf("%d\n", args);
	return (NULL);
	// while (line[i])
	// {
		
	// }
}

int main()
{
	// char *s = "echo \"hi how are u\" a ftah"; 
	// // printf("echo "$pwd"   heehehhehheh");
	
	
		
	// printf("%s\n", "echo " $pwd "  heehehhehheh");


	split_without_quotes("echo  \"$pwd\" fgljahskg \"djkshfka dhjs   \"  'hello there papy'\"    fladjsh\"  djlldksheehehhehheh");
}