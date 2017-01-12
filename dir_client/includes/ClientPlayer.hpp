#ifndef CLIENTPLAYER_HPP
# define CLIENTPLAYER_HPP

# include "Client.hpp"

class ClientPlayer
{
	public:
		ClientPlayer();
		~ClientPlayer();
		void						init(int x, int y);

	private:
		int							posX;
		int							posY;
		int							level;
		int							lifeUnits;
		std::list<std::string>		inventory;
		std::list<std::string>		vision;
};

#endif
