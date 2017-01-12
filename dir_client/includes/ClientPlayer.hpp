#ifndef CLIENTPLAYER_HPP
# define CLIENTPLAYER_HPP

# include "Client.hpp"

class ClientPlayer
{
	public:
		ClientPlayer(std::string teamName);
		~ClientPlayer();

		void						printStat();

		std::string					teamName;
		int							worldSizeX;
		int							worldSizeY;
		int							teamSlots;

	private:
		int							level;
		int							lifeUnits;
		std::list<std::string>		inventory;
		std::list<std::string>		vision;
};

#endif
