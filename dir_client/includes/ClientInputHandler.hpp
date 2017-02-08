#ifndef CLIENTINPUTHANDLER_HPP
# define CLIENTINPUTHANDLER_HPP

# include "Client.hpp"

class ClientInputHandler
{
	public:
		ClientInputHandler(int argc, char **argv);
		~ClientInputHandler();
	
		void			CheckInputFormat(int argc, char **argv);
		void			ParseInput(int argc, char **argv);
		void			FillInput(int argc, char **argv);

		string		teamName;
		string		hostName;
		int				port;
};

#endif
