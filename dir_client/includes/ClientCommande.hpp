#ifndef CLIENTCOMMANDE_HPP
# define CLIENTCOMMANDE_HPP

# include "Client.hpp"

class ClientCommande
{
	public:
		ClientCommande();
		~ClientCommande();

		void												receiveResponse(std::string response);

		virtual void										avance() = 0;
		/*virtual void										droite() = 0;
		virtual void										gauche() = 0;
		virtual void										voir() = 0;*/
		
		virtual void										avanceRes(std::string response) = 0;
		/*virtual void										droiteRes(std::string response) = 0;
		virtual void										gaucheRes(std::string response) = 0;
		virtual void										voirRes(std::string response) = 0;*/

		ClientBuffer										*bufferSend;

	protected:
		std::deque<void (ClientCommande::*)(std::string)>	cmdWaitingResult;

};

#endif