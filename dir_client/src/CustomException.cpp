#include "client.hpp"

CustomException::CustomException(const string& what_arg) : runtime_error(what_arg)
{
	if (errno != 0)
	{
		std::cout << std::endl << "errno: " << std::strerror(errno) << std::endl;
	}
}
