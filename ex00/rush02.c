// rush02.c
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

typedef struct s_dict_entry
{
	char *key;
	char *value;
}	t_dict_entry;

//funcoes auxiliares.

int ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return i;
}

int	ft_strcmp(char *s1, char *s2)
{
	int i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

int	ft_atoi(char *str)
{
	int i = 0;
	int num = 0;

	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return num;
}

void ft_itoa(int num, char *buffer)
{
	int len = 0, temp = num;

	if (num == 0)
	{
		buffer[0] = '0';
		buffer[1] = '\0';
		return;
	}

	while (temp)
	{
		len++;
		temp /= 10;
	}

	buffer[len] = '\0';
	while (num)
	{
		buffer[--len] = (num % 10) + '0';
		num /= 10;
	}
}
// Essa função percorre o dicionário armazenado em um array de structs (t_dict_entry) e busca um número específico (como string).
char *dict_lookup(t_dict_entry *dict, int entries_count, char *number)
{
	int i = 0;
	while (i < entries_count)
	{
		if (ft_strcmp(dict[i].key, number) == 0)
			return dict[i].value;
		i++;
	}
	return NULL; // não encontrado
}

// Essa função recebe um número inteiro e o converte em texto por extenso.
void print_number_text(t_dict_entry *dict, int entries_count, int num)
{
	char num_str[20];
	char *text;

	if (num <= 20 || (num < 100 && num % 10 == 0))
	{
		ft_itoa(num, num_str);
		text = dict_lookup(dict, entries_count, num_str);
		if (text)
			write(1, text, ft_strlen(text));
	}
	else if (num < 100)
	{
		char tens_str[20], units_str[20];
		ft_itoa((num / 10) * 10, tens_str);
		ft_itoa(num % 10, units_str);
		char *tens = dict_lookup(dict, entries_count, tens_str);
		char *units = dict_lookup(dict, entries_count, units_str);
		if (tens && units)
		{
			write(1, tens, ft_strlen(tens));
			write(1, " ", 1);
			write(1, units, ft_strlen(units));
		}
	}
	else if (num < 1000)
	{
		char hundred_str[20];
		ft_itoa(num / 100, hundred_str);
		char *hundred = dict_lookup(dict, entries_count, hundred_str);
		char *hundred_text = dict_lookup(dict, entries_count, "100");
		if (hundred && hundred_text)
		{
			write(1, hundred, ft_strlen(hundred));
			write(1, " ", 1);
			write(1, hundred_text, ft_strlen(hundred_text));
			if (num % 100 != 0)
			{
				write(1, " ", 1);
				print_number_text(dict, entries_count, num % 100);
			}
		}
	}
}

// A função count_entries percorre o conteúdo lido do dicionário (em formato de string) e conta quantas linhas válidas
int	count_entries(char *dict_content)
{
	int i = 0;
	int count = 0;

	while (dict_content[i])
	{
		if (dict_content[i] != '\n' && (dict_content[i + 1] == '\n' || dict_content[i + 1] == '\0'))
			count++;
		i++;
	}
	return count;
}

// Esta função percorre o conteúdo do dicionário (dict_content), linha por linha, e armazena cada entrada (chave e valor) em uma estrutura de dados (t_dict_entry) alocada dinamicamente.
t_dict_entry *store_dict_entries(char *dict_content, int entries_count)
{
	t_dict_entry *dict;
	int i = 0, entry = 0;

	dict = (t_dict_entry *)malloc(sizeof(t_dict_entry) * entries_count);
	if (!dict)
		return NULL;

	while (dict_content[i] && entry < entries_count)
	{
		// pula '\n' extras
		while (dict_content[i] == '\n')
			i++;

		if (!dict_content[i])
			break;

		// aloca chave e valor
		char key[256];
		int k = 0;
		while (dict_content[i] && dict_content[i] != ':' && k < 255)
			key[k++] = dict_content[i++];
		key[k] = '\0';

		if (dict_content[i] == ':')
			i++;
		while (dict_content[i] == ' ')
			i++;

		char value[256];
		int v = 0;
		while (dict_content[i] && dict_content[i] != '\n' && v < 255)
			value[v++] = dict_content[i++];
		value[v] = '\0';

		// Agora, aloca exatamente o espaço necessário para a chave e o valor
		dict[entry].key = malloc(k + 1);
		dict[entry].value = malloc(v + 1);

		// copia a chave e o valor para o struct
		int idx;
		for (idx = 0; idx <= k; idx++)
			dict[entry].key[idx] = key[idx];
		for (idx = 0; idx <= v; idx++)
			dict[entry].value[idx] = value[idx];

		entry++;

		if (dict_content[i] == '\n')
			i++;
	}

	return dict;
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

// Função para validar de validação de input
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
	char			*dict_content;
	t_dict_entry	*dict_entries;
	int				entries_count, i, num;

	if (argc != 2 || !is_numeric(argv[1]))
	{
		write(1, "Error\n", 6);
		return (1);
	}

	num = ft_atoi(argv[1]);

	dict_content = read_dict("numbers.dict");
	if (!dict_content)
	{
		write(1, "Dict Error\n", 11);
		return (1);
	}

	entries_count = count_entries(dict_content);
	dict_entries = store_dict_entries(dict_content, entries_count);
	if (!dict_entries)
	{
		write(1, "Dict Error\n", 11);
		free(dict_content);
		return (1);
	}

	print_number_text(dict_entries, entries_count, num);
	write(1, "\n", 1);

	i = 0;
	while (i < entries_count)
	{
		free(dict_entries[i].key);
		free(dict_entries[i].value);
		i++;
	}
	free(dict_entries);
	free(dict_content);
	return (0);
}
