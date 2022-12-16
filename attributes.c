/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   attributes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchahid <hchahid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 12:12:04 by hchahid           #+#    #+#             */
/*   Updated: 2022/09/17 16:28:21 by hchahid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	modify_attr()
{
	struct termios tp;

	if (tcgetattr(0, &tp) == -1)
		write(2, "tcgetattr error\n", 16);
	tp.c_lflag &= ~ECHOCTL;
	if (tcsetattr(0, TCSAFLUSH, &tp) == -1)
		write(2, "tcsetattr error\n", 16);
}
