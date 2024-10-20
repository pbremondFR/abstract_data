#include "unistd.h"
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <deque>
#include <string>
#include <iostream>

void	print_nline(FILE *f, int count)
{
	char *line = NULL;
	size_t n = 0;
	for (int i = 0; i < count; i++)
	{
		if (getline(&line, &n, f) == -1)
			return;
		std::cout << line;
	}
}

int main(int ac, char **av)
{
	if (ac < 3)
	{
		std::cerr << "Invalid number of arguments" << std::endl;
		return 1;
	}
	int context_size = 10;
	if (ac == 4)
		context_size = atoi(av[3]);
	FILE *a = popen(av[1], "r");
	FILE *b = popen(av[2], "r");

	std::deque<std::string> context;
	char	*linea = NULL, *lineb = NULL;
	size_t	na = 0, nb = 0;
	while (1)
	{
		int reta = getline(&linea, &na, a);
		int retb = getline(&lineb, &nb, b);
		if (reta == -1 && retb == -1)
			break;
		if (reta != retb || strcmp(linea, lineb))
		{
			for (auto &line : context)
				std::cout << line;
			std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> STREAMS DIFFERS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
			std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> A <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
			if (reta != -1)
			{
				std::cout << linea;
				print_nline(a, context_size);
			}
			std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> B <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
			if (retb != -1)
			{
				std::cout << lineb;
				print_nline(b, context_size);
			}
			break;
		}
		context.push_back(std::string(linea));
		if (context.size() > context_size)
			context.pop_front();
	}
	if (feof(a) && feof(b))
		std::cout << "OK" << std::endl;
	else
		std::cout << "KO" << std::endl;
}