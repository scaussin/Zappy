#ifndef CLIENTPLAYER_HPP
# define CLIENTPLAYER_HPP

# include "Client.hpp"

class ClientPlayer : public ClientCommande
{
	public:
		ClientPlayer(std::string teamName);
		~ClientPlayer();

		void						printStat();

		std::string					teamName;
		int							worldSizeX;
		int							worldSizeY;
		int							teamSlots;

		virtual void				avance();
		virtual void				droite();
		virtual void				gauche();
		virtual void				voir();

	private:
		virtual void				avanceCallback(std::string response);
		virtual void				droiteCallback(std::string response);
		virtual void				gaucheCallback(std::string response);
		virtual void				voirCallback(std::string response);

		int							level;
		int							lifeUnits;
		std::list<std::string>		inventory;
		std::list<std::string>		vision;
};

#endif
