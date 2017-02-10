#ifndef CLIENTBUFFER_HPP
# define CLIENTBUFFER_HPP

# include "client.hpp"

class ClientBuffer
{
	public:
		ClientBuffer();
		~ClientBuffer();
		string	getFirstMsg();
		string	getBuffer();
		void		erase(int len);
		int			getLen();
		void		pushMsg(string msg);
		void		pushBuffer(char *to_write, int len);

	private:
		char		buffer[BUFF_SIZE];
		int			len;
		int			start;
};

#endif