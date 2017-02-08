#ifndef CUSTOMEXCEPTION_HPP
# define CUSTOMEXCEPTION_HPP

#include "Client.hpp"

class CustomException : public runtime_error
{
	public:
		CustomException(const string& what_arg);
};

#endif 
