#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
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
 *	*_getenv - search string
 *	@name: String to search
 *	Return: Pointer to the string
 */

char *_getenv(const char *name)
{
	char *ptr = NULL;
	char **list = NULL;
	int i = 0, j = 0;

	list = __environ;

	for (i = 0; list[i] != NULL; i++)
	{
		for (j = 0; name[j] == list[i][j]; j++)
		{
		}

		if (name[j] == '\0' && list[i][j] == '=')
		{
			ptr = list[i];
			return (ptr);
		}

	}

	if (name[j] == '\0' && list[i][j] == '=')
		ptr = list[i];
	else
		ptr = NULL;

	return (ptr);
}

/**
 *	*_search - search string
 *	@name: String to search
 *	Return: Pointer to the string
 */

char **_search(char *str)
{
	char *_env = NULL;
	char **PATH = NULL;
	char **PATHS = NULL;
	int i = 0;

	_env = _getenv(str);

	if (_env == NULL)
		printf("La variable de entorno no Existe\n");

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
		while(PATHS[i] != NULL)
		{
			printf("token posicion[%d] %s\n", i, PATHS[i]);
			i++;
		}
		printf("token posicion[%d] %s\n", i, PATHS[i]);
	}
	free(PATHS);
	return (PATHS);
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
	int i = 0;
	int pid, status;

	while (1)
	{

		line = prompt("prueba shell (&) ");
		arguments = divide_line(line, " ");

		while (arguments[i] != NULL)
		{
			printf("%s\n", arguments[i]);
			i++;
		}

		pid = fork();

		if (pid == 0)
		{
			execve(arguments[0], arguments, NULL);
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
	char **paths;
	
	paths = _search("PATH");
	//start_loop();
	return (0);
}
