#include "../includes/serveur.h"

void	exit_error(char *error_log)
{
	perror(error_log);
	exit(errno);
}

/*
**	Safe mallocating.
*/

void	*s_malloc(size_t size)
{
	void	*ret;

	if (!(ret = malloc(size)))
		exit_error("malloc()");
	return (ret);
}

void	replace_nl(char *str, int len)
{
	int i;

	i = 0;
	while (i < len)
	{
		if (str[i] == '\n')
			str[i] = '*';
		i++;
	}
}

char	*str_concat_realloc1(char *str1, char *str2)
{
	char	*ret;
	int		len1;
	int		len2;
	int		i;

	len1 = strlen(str1);
	len2 = strlen(str2);
	ret = (char *)s_malloc(sizeof(char) * (len1 + len2 + 1));
	i = -1;
	while (++i < len1)
		ret[i] = str1[i];
	i = -1;
	while (++i < len2)
		ret[i + len1] = str2[i];
	ret[i + len1] = '\0';
	free(str1);
	return (ret);
}

/*void	logs(int type, char *log)
{
	char *type_str;

	if (type == 0)
	{
		type_str = "\x1B[36m[NETWORK]";
	}
	else if (type == 1)
	{
		type_str = "\x1B[32m[INFO]";
	}
	replace_nl(log);
	printf("%s%s%s\n", type_str, log, KRESET);
}*/

/*
**	A command finishes by '\n'. This function returns the length until that '\n'
*/

int		get_len_cmd(char *str)
{
	int		len;

	len = 0;
	while (*str && *str != '\n')
	{
		len++;
		str++;
	}
	if (!(*str))
		return (-1);
	return (len);
}

char	*get_cmd_trim(char *str)
{
	int		len;

	if ((len = get_len_cmd(str)) < 0)
	{
		return (NULL);
	}
	return strndup(str, len);
}

int		modulo(int a, int b)
{
	int	ret;

	ret = a % b;
	return (ret < 0 ? ret + b : ret);
}

void	print_buff(t_buffer buff)
{
	printf("buff [%s]\n", buff.buff);
	printf("start: %d len: %d\n", buff.start, buff.len);
	fflush(stdout);
}

/*
**	Display functions for server prints.
*/

void print_send(int sock, char *str, int len)
{
	printf(KYEL " Sending to sock %d: " KRESET, sock);
	replace_nl(str, len);
	printf("[%.*s]\n", len, str);
}

void print_send_gfx(char *str)
{
	printf(KMAG " Sending to gfx : " KRESET);
	printf("[%.*s]\n", (int)strlen(str), str);
}

void print_receive(int sock, char *str, int len)
{
	//str[len] = '\0'; // fix print ouf of bond.
	printf(KBLU " Receiving from sock %d: " KRESET, sock);
	replace_nl(str, len);
	printf("[%.*s] len: %d\n", len, str, len);
}
