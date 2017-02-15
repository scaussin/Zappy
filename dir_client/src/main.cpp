#include "client.hpp"

int		main(int argc, char **argv)
{
	ClientController	client(argc, argv);
	 srand (time(NULL));

	if (client.communication->isConnected)
	{
		client.mainLoop();	
	}
	return (0);
}

/*vector<string>	strSplit(string str, string delimiters)
{
	char	*token;
	char	*dup;
	vector<string> ret;

	dup = strdup(str.c_str());
	token = strtok(dup, delimiters.c_str());
	while (token)
	{
		ret.push_back(token);
		token = strtok(NULL, delimiters.c_str());
	}
	free(dup);
	return (ret);
}*/

vector<string>	strSplit(string str, char c)
{
	vector<string> ret;
	string a;
	int i = 0;

	i = 0;
	while (1)
	{
		a = "";
		while (str[i] && str[i] != c)
		{
			a.push_back(str[i]);
			i++;
		}
		if (str[i] == 0)
		{
			ret.push_back(a);
			break;
		}
		else
		{
			ret.push_back(a);
			i++;
		}
	}
	return (ret);
}

