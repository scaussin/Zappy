#ifndef CUSTOMEXCEPTION_HPP
# define CUSTOMEXCEPTION_HPP

#include "Client.hpp"

class CustomException : public std::runtime_error
{
	public:
		CustomException(const std::string& what_arg);
};

#endif 
