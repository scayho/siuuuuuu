/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   if_directory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchahid <hchahid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 17:55:07 by hchahid           #+#    #+#             */
/*   Updated: 2022/09/13 18:08:15 by hchahid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_directory(char	*buf)
{
	struct stat st;

	stat(buf, &st);
	// if ((st.st_mode & S_IFMT) == S_IFDIR)
	if (S_ISDIR(st.st_mode))
	{
		printf("%s: is a directory\n", buf);
		return (1);
	}
	return (0);
}