
#include ""

//
//	*** get chained list with tokens from parsing part ***
//		token can be 3 things:
//
//		word : check for command or file and if command, execute
//		pipe : fork, 2 new children until both sides of pipe processed, then back to parent
//		redirection : use of dup2? redirect input or output of file
//
//		iterate until pipe, then execute both sides
//		if no pipe, just execute
//


char	*is_built_in(char *str)
{
	char	**paths;
	int		i;
	char	*pathname;

	paths = ft_split(getenv("PATH"), ":");
	i = 0;
	while (paths[i])
	{
		pathname = ft_strjoin(paths[i], str);
		if (acces(pathname, X_OK))
			return (pathname);
		i++;
	}
	return (NULL);
}

//void	start_exec(t_node **list)
//{
//	while (/*token exists*/)
//	{
//		if (/*next token == pipe*/)
//		{
//			/*fork
//			get cmd left
//			get cmd right*/
//		}
//		else if (/*token == redir*/)
//		{}
//		else if (/*token == word*/)
//		{
//			/*get_command()*/
//		}
//	}
//}

int	main()
{
	char	*path;

	path = getenv("PATH");
	printf("paths : %s\n", path);
}
