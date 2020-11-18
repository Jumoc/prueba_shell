#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

/**
 *	string_size - size of a string
 *	@str: string to be counted
 *
 *	Return: size of the string
 */

int string_size(char *str)
{
	int size = 0;

	if (str)
	{
		while (str[size] != '\0')
		{
			size++;
		}
	}

	return (size);
}

/**
 *	prompt - prompts a message and gets user input
 *	@text: text to be displayed as prompt
 *
 *	Return: arguments as input recieved
 */

char *prompt(char *text)
{
	char *line = NULL;
	size_t size;
	int prompt_size = 0, i = 0;

	if (!text)
	{
		perror("Prompt message can't be NULL");
		return (NULL);
	}

	prompt_size = string_size(text);
	/*
	*	must use #include <unistd.h>
	*/
	if (write(STDOUT_FILENO, text, prompt_size) == -1)
	{
		perror("Prueba1- ");
	}
	/*
	*	Liberar!
	*/
	if (getline(&line, &size, stdin) == -1)
	{
		perror(NULL);
		exit(-1);
	}

	while (line[i] != '\0')
	{
		if (line[i] == '\n' || line[i] == EOF)
		{
			line[i] = '\0';
		}
		i++;
	}
	return (line);
}

/**
 *	divide_line - divides a line
 *	@line: line to be divided
 *	@delim: delimitator
 *
 *	Return: array of arguments
 */

char **divide_line(char *line, char *delim)
{
	char **tokens = NULL;
	char *current = NULL, *aux = NULL;
	int i = 0, j = 0;

	/*
	*	Liberar!
	*/

	tokens = malloc(32 * sizeof(char *));

	if (!tokens)
		return (NULL);

	current = line;
	aux = line;
	/*
	*	seguirle a la lógica
	*/
	while (current[i] != '\0')
	{
		if (current[i] == *delim)
		{
			current[i] = '\0';
			tokens[j] = aux;
			aux = (current + i + 1);
			j++;
		}
		i++;
	}
	tokens[j] = aux;
	tokens[j + 1] = NULL;

	return (tokens);
}

/**
 * *_strcpy - prints n numbers of an array
 *
 * @dest: string to be printed
 * @src: number of elements to be printed
 *
 * Return: The string copied
*/
char *_strcpy(char *dest, char *src)
{
	char *firstValue = dest;
	for (; *src != '\0'; src++)
	{
		*dest = *src;
		dest++;
	}
	*dest = '\0';
	return (firstValue);
}

/**
 *	*_getenv - search string
 *	@name: String to search
 *	Return: Pointer to the string
 */

char *_getenv(const char *name)
{
	char *ptr = NULL;
	char **list = NULL;
	int size = 0;
	int i = 0, j = 0;

	list = __environ;

	for (i = 0; list[i] != NULL; i++)
	{
		for (j = 0; name[j] == list[i][j]; j++)
		{
		}

		if (name[j] == '\0' && list[i][j] == '=')
		{
			/*liberar malloc*/
			size = string_size(list[i]);
			ptr = malloc(size + 1);
			ptr = _strcpy(ptr, list[i]);
			return (ptr);
		}
	}

	if (name[j] == '\0' && list[i][j] == '=')
	{
		size = string_size(list[i]);
		ptr = malloc(size + 1);
		_strcpy(ptr, list[i]);
	}

	return (ptr);
}

/**
 *	divide_path - search
 *	@name: String to search
 *	Return: Pointer to the string
 */

char **divide_path(char *str)
{
	char *_env = NULL;
	char **PATH = NULL;
	char **PATHS = NULL;
	int i = 0;

	_env = _getenv(str);

	if (_env == NULL)
		perror(NULL);

	else
	{
		/*
		 * liberar
		 */
		PATH = divide_line(_env, "=");
		/*
		 * liberar
		 */
		PATHS = divide_line(PATH[1], ":");
	}
	return (PATHS);
}

char *concat_path(char *command, int index)
{
	char **paths = NULL;
	char *concat = NULL;
	int command_size = 0, path_size = 0, paths_size = 0;
	int i = 0, j = 0;

	paths = divide_path("PATH");

	while (paths[paths_size] != NULL)
	{
		paths_size++;
	}

	if (paths_size < index)
		return (NULL);

	if (command)
		command_size = string_size(command);

	if (paths[index])
		path_size = string_size(paths[index]);

		/*liberar concat*/

	concat = malloc((path_size + command_size + 2) * sizeof(char));

	while (i < path_size)
	{
		concat[i] = paths[index][i];
		i++;
	}
	concat[i] = '/';
	i++;
	while (j < command_size)
	{
		concat[i] = command[j];
		j++;
		i++;
	}
	concat[i] = '\0';
	return (concat);
}

/**
 *	start_loop - starts the inifinite loop
 *
 *	Return: void
 */

void start_loop(void)
{
	char *line = NULL;
	char **arguments = NULL;
	char *path = NULL;
	int i;
	int pid, status, pr, path_status;
	struct stat *statbuf;

	while (1)
	{
		i = 0;
		line = prompt("prueba shell (&) ");
		arguments = divide_line(line, " ");

		pr = stat(arguments[0], statbuf);

		if (pr == -1)
		{
			/*
			 *	hacer concatenación
			*/
			do 
			{
				path = concat_path(arguments[0], i);
				path_status = stat(path, statbuf);

				if(path_status == 0)
				{
					break;
				}
				i++;
			}
			while (path != NULL);
		} else {
			path = arguments[0];
		}
		
		pid = fork();

		if (pid == 0)
		{
			execve(path, arguments, NULL);
		} else
		{
			wait(&status);
		}
	}
}

/**
 *	main - main function
 *	@argv: number of arguments passed
 *	@argc: arguments passed
 *	@env: environment
 *
 *	Return: 0 always
 */
int main(int argv, char **argc, char **env)
{
	start_loop();
	return (0);
}
