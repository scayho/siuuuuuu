#include "minishell.h"


void	sighandle(int	sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		// rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();	
	}
}

void	sigInit()
{
	signal(SIGINT, sighandle);
	signal(SIGQUIT, SIG_IGN);
}

void	sigDefault()
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

