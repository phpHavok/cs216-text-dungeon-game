#ifndef GAME_H_
#define GAME_H_

#include <ncurses.h>
#include <vector>
#include <sstream>
#include "Player.h"

class DungeonLevel;
class Creature;
class Item;

class Game {
	public:
		virtual ~Game();
		static Game & getInstance();
		virtual void gameLoop();
		virtual std::stringstream & os();
	private:
		Game();
		void movePlayerToDownstairs();
		void movePlayerToUpstairs();
		void movePlayer( int x, int y );
		void movePlayerUpDungeon();
		void movePlayerDownDungeon();
		void playerPickUpItem();
		void playerDropItem();
		void playerEquipItem();
		void playerUseItem();
		void playerViewInventory();
		void spawnCreatures();
		void spawnItems();
		int playerVisibilityMap( int x, int y );
		std::vector<DungeonLevel*> mMaps;
		std::vector<Creature*> mCreatures;
		std::vector<Item*> mItems;
		Player mPlayer;
		int mCurrentMap;
		WINDOW * mWindowMap;
		WINDOW * mWindowInteractive;
		std::stringstream mOS;
		enum {
			CP_DARK = 0x01
		};
};

#endif
