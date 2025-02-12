#ifndef PIPEX_REFACTORED_H
#define PIPEX_REFACTORED_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include "libft.h"

void    run_command(int input_fd, int output_fd, char *command, char **env);
int     first_process(char *input_file, char *command, char **env);
pid_t   last_process(int input_fd, char *command, char *output_file, char **env);
int     wait_for_processes(pid_t last_pid, int process_count);
void    exit_error(char *message);
char    **extract_path_variable(char **env);
char    *get_command_path(char *command, char **env);

#endif
