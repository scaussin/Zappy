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
