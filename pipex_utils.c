#include "pipex.h"

void	exit_error(char *message)
{
	perror(message);
	exit(1);
}

void	free_arrays(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

char **extract_path_variable(char **env)
{
	char **path_directories;
	int   i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "PATH=", 5))
		{
			path_directories = ft_split(&env[i][5], ':');
			if (!path_directories)
				exit_error("Memory allocation failed");
			return (path_directories);
		}
		i++;
	}
	return (NULL);
}

char *get_command_path(char *command, char **env)
{
	char **path_directories;
	char *temp;
	char *full_path;
	int   i;

	if (ft_strchr(command, '/'))
		return (command);
	path_directories = extract_path_variable(env);
	temp = ft_strjoin("/", command);
	i = 0;
	while (path_directories && path_directories[i])
	{
		full_path = ft_strjoin(path_directories[i], temp);
		if (access(full_path, F_OK) == 0)
		{
			free_arrays(path_directories);
			free(temp);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_arrays(path_directories);
	free(temp);
	return (command);
}

int	wait_for_processes(pid_t last_pid, int process_count)
{
	int	status;
	int	temp_status;
	int	i;

	status = 1;
	i = 0;
	while (i < process_count)
	{
		if (waitpid(-1, &temp_status, 0) == last_pid)
			status = (temp_status >> 8) & 0xff;
		i++;
	}
	return (status);
}
