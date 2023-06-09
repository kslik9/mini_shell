/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kslik <kslik@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 16:38:09 by kslik             #+#    #+#             */
/*   Updated: 2023/05/23 16:49:34 by kslik            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	is_valid_input(char *input)
{
	if (!input)
		exit(1);
	if (is_empty(input))
		return (0);
	return (1);
}

void	tokenize_parse_execute(char *input, t_my_env **my_env, int *exit_code)
{
	t_token	*tokens;
	t_tree	*tree;

	if (!is_valid_input(input))
	{
		free(input);
		return ;
	}
	tokens = lets_tokenize(input);
	free(input);
	if (!check_tokens(tokens, exit_code))
	{
		free_tokens(&tokens);
		return ;
	}
	tree = lets_parse(&tokens);
	free_tokens(&tokens);
	lets_execute(tree, my_env, is_single_cmd(tree), exit_code);
	free_tree(&tree);
}

void	ctrl_c_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_redisplay();	}
}

void	my_env_cop(char **env, t_my_env **my_env, int *exit_code)
{
	copy_env(my_env, env);
	*exit_code = 0;
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int c, char **arg, char **env)
{
	char		*input;
	char		*our_shell;
	t_my_env	*my_env;
	static int	exit_code;

	my_env = NULL;
	my_env_cop(env, &my_env, &exit_code);
	(void)arg;
	(void)c;
	while (1)
	{
		our_shell = get_cdir(exit_code);
		input = readline(our_shell);
		free(our_shell);
		if (!is_valid_input(input))
		{
			free(input);
			continue ;
		}
		if (ft_strlen(input) > 0)
			add_history(input);
		input = expandini(input, my_env, exit_code);
		tokenize_parse_execute(input, &my_env, &exit_code);
	}
	free_my_env(&my_env);
}
