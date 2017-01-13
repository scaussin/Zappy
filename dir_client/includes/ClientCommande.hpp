#ifndef CLIENTCOMMANDE_HPP
# define CLIENTCOMMANDE_HPP

# include "Client.hpp"

class ClientCommande
{
	public:
		ClientCommande();
		~ClientCommande();

		void												receiveCallbackCommande(std::string response);

		virtual void										avance() = 0;
		virtual void										droite() = 0;
		virtual void										gauche() = 0;
		virtual void										voir() = 0;
		
		virtual void										avanceCallback(std::string response) = 0;
		virtual void										droiteCallback(std::string response) = 0;
		virtual void										gaucheCallback(std::string response) = 0;
		virtual void										voirCallback(std::string response) = 0;

		ClientBuffer										*bufferSend;

	protected:
		std::deque<void (ClientCommande::*)(std::string)>	stackCallbackCommandes;

};

#endif