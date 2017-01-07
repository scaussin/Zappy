#include "../includes/serveur.h"

void	exit_error(char *error_log)
{
	perror(error_log);
	exit(errno);
}

void	*s_malloc(size_t size)
{
	void	*ret;

	if (!(ret = malloc(size)))
		exit_error("malloc()");
	return (ret);
}

void	replace_nl(char *str)
{
	while (*str)
	{
		if (*str == '\n')
			*str = '*';
		str++;
	}
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

t_match_lexer *init_match_lexer()
{
	t_match_lexer *match_lexer;

	match_lexer = s_malloc(sizeof(t_match_lexer) * SIZE_MATCH_LEXER);
	match_lexer[0].name = "avance";
	match_lexer[0].func = cmd_avance;
	match_lexer[1].name = "droite";
	match_lexer[1].func = cmd_droite;
	return (match_lexer);
}

void	print_buff(t_buffer buff)
{
	printf("buff [%s]\n", buff.buff);
	printf("start: %d len: %d\n", buff.start, buff.len);
	fflush(stdout);
}