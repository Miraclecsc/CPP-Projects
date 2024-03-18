#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX_LINE 80
char** init_args (char** args, char* inst, char* last)
{
	int args_id = 0, inst_id = 0;
	char now = inst[inst_id];
	int pre = 0;

	while (now != '\n' && now != EOF)
	{
		if (inst_id == 0)
		{
			free (args[0]);
			args[0] = (char *) malloc (MAX_LINE * sizeof (char));
			memset (args[0], 0, sizeof (args[0]));
		}
		if (now == ' ')
		{
			if (pre == 0)
			{
				args_id++;
				args[args_id] = (char* ) malloc (MAX_LINE * sizeof (char));
				pre = 1;
			}
		}
		else
		{
			pre = 0;
			strncat (args[args_id], &now, 1);
		}
		inst_id ++;
		now = inst[inst_id];
	}
	if (args[args_id] != NULL)
		strcpy (last, args[args_id]);
	if (strcmp (last, "&") == 0)
		args[args_id] = NULL;
}
char **init_buff (char** args, char** buffer, char* last)
{
	if (args[0] == NULL || strcmp (args[0], "!!") == 0)
		return NULL;
	int i = 0;
	for (i = 0; i < MAX_LINE / 2 + 1; i++)
	{
		if (args[i] == NULL)
			break;
		buffer[i] = (char*) malloc (MAX_LINE * sizeof (char));
		strcpy (buffer[i], args[i]);
	}
	if (strcmp (last, "&") == 0)
	{
		buffer[i] = (char*) malloc (MAX_LINE * sizeof (char));
		buffer[i] = "&";
	}
}

int main(void)
{
	// init_0:
	char *args[MAX_LINE/2 + 1] = {NULL};	
	char *buffer[MAX_LINE/2 + 1] = {NULL};
		
	while (1)
	{   
		//init_1
		char * inst = (char*) malloc (MAX_LINE * sizeof(char));
		char * last = (char*) malloc (MAX_LINE * sizeof(char));
		for (int i = 0; i < MAX_LINE / 2 + 1; i++)
			args[i] = NULL;
		//part1: get args and inst
		printf ("osh>");
		fflush (stdout);
		fgets (inst, MAX_LINE, stdin);
		init_args (args, inst, last);
		init_buff (args, buffer, last);
		free (inst);
		free (last);
		//part2: check if !!
		if (args[0] != NULL && strcmp (args[0], "!!") == 0)
		{
			free (args[0]);
			args[0] = NULL;
			if (buffer[0] == NULL)
			{
				printf ("No commands in history!\n");
				continue;
			}
			else
			{
				for (int i = 0; i < MAX_LINE / 2 + 1; i++)
				{
					if (buffer[i] == NULL)
						break;
					else
					{
						args[i] = (char*) malloc (MAX_LINE * sizeof (char));
						strcpy (args[i], buffer[i]);
					}
					printf ("%s ",buffer[i]);
				}
				printf ("\n");
			}
		}
		//part3: check if exit		
		if (args[0] != NULL && strcmp (args[0], "exit") == 0)
			break;
		//part4: pipe
			//4.1 pipe init
		int pipe_id = 0;
		for (int i = 0; i < MAX_LINE / 2 + 1; i++)
		{
			if (args[i] != NULL && strcmp (args[i], "|") == 0)
				pipe_id = i;
		}
			//4.2 create pipe
		if (pipe_id != 0)
		{
			pid_t pid;
			pid = fork();

			if (pid != 0)
				wait (NULL);
			else
			{
				int flag[2];
				pid_t pid;

				if (pipe(flag) == -1)
				{
					fprintf (stderr, "Pipe failed");
					break;
				}

				pid = fork();
				if (pid > 0)
				{
					for (int i = pipe_id; i < MAX_LINE / 2 + 1; i++)
					{
						free (args[i]);
						args[i] = NULL;
					}
					close (flag[0]);
					dup2 (flag[1], STDOUT_FILENO);
					execvp (args[0], args);
				}
				else if (pid == 0)
				{
					strcpy (args[0], args[pipe_id + 1]);
					for (int i = 1; i < MAX_LINE / 2 + 1; i++)
					{
						free (args[i]);
						args[i] = NULL;
					}
					close (flag[1]);
					dup2 (flag[0], STDIN_FILENO);
					execvp (args[0], args);
				}
			}
		}
			//4.3 no pipe,then redirection
		else
		{
			pid_t pid;
			pid = fork();
			if (pid == 0)
			{
				for (int i = 0; i < MAX_LINE / 2 + 1; i++)
				{
					if (args[i] != NULL && (strcmp (args[i], "<") == 0 || strcmp (args[i], ">") == 0))
					{
						//4.3.1 input_redirection
						if (strcmp (args[i], "<") == 0)
						{
							int flag = open (args[i + 1], O_RDONLY, 0644);
							free (args[i]),free (args[i+1]);
							args[i] = args[i + 1] = NULL;
							dup2 (flag, STDIN_FILENO);
						}
						//4.3.2 output_redirection
						else
						{
							char file[MAX_LINE];
							strcpy (file, args[i + 1]);
							int flag = open (file, O_RDWR | O_NOCTTY | O_NDELAY);
							if (flag == -1)
								flag = creat (file, S_IRWXU);
							free (args[i]), free (args[i+1]);
							args[i] = args[i+1] = NULL;
							dup2 (flag, STDOUT_FILENO);						
						}
					}
				}
				execvp (args[0], args);
				break;
			}
			else
			{
				if (strcmp (last, "&") != 0)
					wait (NULL);
			}
		}
	}
	//part5: _init
	for (int i = 0; i < MAX_LINE / 2 + 1; i++)
		free (args[i]), free(buffer[i]);
	return 0;
}
