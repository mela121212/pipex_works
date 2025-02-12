#include "pipex.h"

void	run_command(int input_fd, int output_fd, char *command, char **env)
{
	char	**args;
	char	*cmd_path;

	if (dup2(input_fd, STDIN_FILENO) == -1)
	{
		close(input_fd);
		close(output_fd);
		exit_error("Error duplicating stdin");
	}
	close(input_fd);
	if (dup2(output_fd, STDOUT_FILENO) == -1)
	{
		close(output_fd);
		exit_error("Error duplicating stdout");
	}
	close(output_fd);
	args = ft_split(command, ' ');
	if (!args)
		exit_error("Memory allocation failed");
	cmd_path = get_command_path(args[0], env);
	execve(cmd_path, args, env);
	exit_error("Command execution failed");
}

int	first_process(char *input_file, char *command, char **env)
{
	pid_t process_id;
	int	  pipe_fd[2];
	int	  input_fd;

	if (pipe(pipe_fd) == -1)
		exit_error("Pipe creation failed");
	process_id = fork();
	if (process_id == -1)
		exit_error("Fork failed");
	else if (process_id == 0)
	{
		close(pipe_fd[0]);
		input_fd = open(input_file, O_RDONLY);
		if (input_fd == -1)
		{
			close(pipe_fd[1]);
			exit_error("Failed to open input file");
		}
		run_command(input_fd, pipe_fd[1], command, env);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

pid_t last_process(int input_fd, char *command, char *output_file, char **env)
{
	pid_t process_id;
	int	  output_fd;

	process_id = fork();
	if (process_id == -1)
		exit_error("Fork failed");
	else if (process_id == 0)
	{
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (output_fd == -1)
		{
			close(input_fd);
			exit_error("Failed to open output file");
		}
		run_command(input_fd, output_fd, command, env);
	}
	close(input_fd);
	return (process_id);
}

int main(int argc, char **argv, char **env)
{
	pid_t second_process;
	int   input_fd;

	if (argc != 5)
	{
		perror("Wrong number of arguments\n");
		return (1);
	}
	input_fd = first_process(argv[1], argv[2], env);
	second_process = last_process(input_fd, argv[3], argv[4], env);
	return (wait_for_processes(second_process, 2));
}
