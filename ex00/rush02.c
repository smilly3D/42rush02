// rush02.c
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h> // somente para testes (permitido durante desenvolvimento)

typedef struct s_dict_entry
{
	char *key;
	char *value;
}	t_dict_entry;


// Função inicial para testar separação das entradas corretamente
void	parse_dict(char *dict_content)
{
	int i = 0;
	while (dict_content[i])
	{
		// pular quebras de linhas extras
		while (dict_content[i] == '\n')
			i++;

		if (!dict_content[i])
			break;

		// obter a chave
		char key[256];
		int k = 0;
		while (dict_content[i] && dict_content[i] != ':' && k < 255)
			key[k++] = dict_content[i++];
		key[k] = '\0';

		// pula ':'
		if (dict_content[i] == ':')
			i++;

		// pula espaços antes do valor
		while (dict_content[i] == ' ')
			i++;

		// obter o valor
		char value[256];
		int v = 0;
		while (dict_content[i] && dict_content[i] != '\n' && v < 255)
			value[v++] = dict_content[i++];
		value[v] = '\0';

		// exibir chave e valor usando apenas write()
		write(1, "Chave: '", 8);
		write(1, key, k);
		write(1, "' - Valor: '", 12);
		write(1, value, v);
		write(1, "'\n", 2);

		// pula quebra de linha final
		if (dict_content[i] == '\n')
			i++;
	}
}


// Função para obter o tamanho do arquivo
int	get_file_size(char *filename)
{
	int		fd;
	int		bytes_read;
	char	buffer;
	int		size;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
	size = 0;
	bytes_read = read(fd, &buffer, 1);
	while (bytes_read > 0)
	{
		size++;
		bytes_read = read(fd, &buffer, 1);
	}
	close(fd);
	return (size);
}

// Função para ler e retornar o conteúdo completo do arquivo
char	*read_dict(char *filename)
{
	int		fd;
	int		size;
	char	*buffer;

	size = get_file_size(filename);
	if (size <= 0)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * (size + 1));
	if (!buffer)
		return (NULL);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		free(buffer);
		return (NULL);
	}
	read(fd, buffer, size);
	buffer[size] = '\0';
	close(fd);
	return (buffer);
}

int is_numeric(char *str)
{
	int i;

	i = 0;
	if(str[0] == '\0')
		return 0;
	while (str[i])
	{
		if(str[i] < '0' || str[i] > '9')
			return 0;
		i++;
	}
	return 1;
}

int	main(int argc, char **argv)
{
	char	*dict_content;

	if (argc != 2 || !is_numeric(argv[1]))
	{
		write(1, "Error\n", 6);
		return (1);
	}

	dict_content = read_dict("numbers.dict");
	if (!dict_content)
	{
		write(1, "Dict Error\n", 11);
		return (1);
	}

	parse_dict(dict_content); // testa o parseamento inicial

	free(dict_content);
	return (0);
}
