#ifndef CLIENTBUFFER_HPP
# define CLIENTBUFFER_HPP

# include "Client.hpp"

class ClientBuffer
{
	public:
		ClientBuffer();
		~ClientBuffer();
		std::string	getFirstMsg();
		std::string	getBuffer();
		void		erase(int len);
		int			getLen();
		void		pushMsg(std::string msg);
		void		pushBuffer(char *to_write, int len);

	private:
		char		buffer[BUFF_SIZE];
		int			len;
		int			start;
};

#endif