/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelahce <abelahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 17:23:29 by abelahce          #+#    #+#             */
/*   Updated: 2022/10/26 17:23:30 by abelahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


int	checkqot(char *str, int dj, int role)
{
	int i;

	i = 0;
	if (role == 1)
	{
		while (str[i] != dj)
		{
			if (str[i] == '\0')
				return (0);
			i++;
		}
	}
	else if (role == 2)
	{
		while (str[i] != dj)
			i++;
		return (i);
	}
	return (1);
}

int	check_qotes(char *str)
{
	int	i;

	i = -1;
	while (str[++i] != '\0')
	{
		if (str[i] == 39)
		{
			if (checkqot(str + 1 + i, 39, 1) == 0)
			{
				printf("problem with the single qoute  \n");
				return (0);
			}
			else
				i += checkqot(str + 1 + i, 39, 2) + 1;
		}
		if (str[i] == 34)
		{
			if (checkqot(str + 1 + i, 34, 1) == 0)
			{
				printf("problem with the double qoute\n");
				return (0);
			}
			else
				i += checkqot(str + 1 + i, 34, 2) + 1;

		}
	}
	return (1);
}
