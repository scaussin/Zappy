#include "../includes/Client.hpp"

CustomException::CustomException(const std::string& what_arg) : std::runtime_error(what_arg)
{	
}
