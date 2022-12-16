/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchahid <hchahid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 18:34:21 by hchahid           #+#    #+#             */
/*   Updated: 2022/11/02 03:10:35 by hchahid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	if (c == '\t' || c == '\n' || c == ' ' || c == '\v'
		|| c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	is_there_file(char *s)
{
	int	i;

	i = 0;
	if (!s[i])
		return (0);
	while (is_space(s[i]) && s[i])
		i++;
	if (s[i] && (s[i] == '<' || s[i] == '>' || s[i] == '|'))
		return (0);
	return (1);
}

int	first_in_line(char *s)
{
	if (s[0] == '|' || s[0] == '&')
		return (1);
	return (0);
}

// int	in_quotes(char *s, char *key)
// {
// 	while (key != s)
// 	{
// 		if (*key == '"' || *key == '\'')
// 			return (1);
// 		key--;
// 	}
// 	return (0);
// }


int	syntax_error(char *s)
{
	// char	*tmp;
	int	i= 0;

	if (s[0] == '/' && !s[1])
		return (printf("/: is a directory\n"));
	if (first_in_line(s))
		return (1);
	i = first_redirection(s);
	if  ( *(s + i) == '>' && !is_there_file(s + i) && !is_quoted(s, s + i))
		return (1);
	else if ((s = ft_strchr(s, '>')) && !is_there_file(s + 1)  && !is_quoted(s, s))
		return (1);
	else if  ((s = ft_strchr(s, '<')) && *(s + 1) == '<' && !is_there_file(s + 2) && !is_quoted(s, s))
		return (1);
	else if ((s = ft_strchr(s, '<')) && !is_there_file(s + 1) && !is_quoted(s, s))
		return (1);
	else if ((s = ft_strchr(s, '|')) && !is_there_file(s + 1) && !is_quoted(s, s))
		return (1);
	return (0);
}
