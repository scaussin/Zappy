#ifndef CUSTOMEXCEPTION_HPP
# define CUSTOMEXCEPTION_HPP

#include "client.hpp"

class CustomException : public runtime_error
{
	public:
		CustomException(const string& what_arg);
};

#endif 
