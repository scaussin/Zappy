#include "client.hpp"

CustomException::CustomException(const string& what_arg) : runtime_error(what_arg)
{	
}
