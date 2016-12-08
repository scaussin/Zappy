#ifndef CLIENTINPUTHANDLER_HPP
# define CLIENTINPUTHANDLER_HPP

class ClientInputHandler
{
	public:
		ClientInputHandler();
		~ClientInputHandler();
	
		void	GetInput(ClientConnection &Connection, int argc, char **argv);
		void	CheckInputFormat(int argc, char **argv);
		void	ParseInput(int argc, char **argv);
		void	FillInput(ClientConnection &Connection, int argc, char **argv);
};

#endif
