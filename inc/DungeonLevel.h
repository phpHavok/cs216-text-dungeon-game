#ifndef DUNGEON_LEVEL_H_
#define DUNGEON_LEVEL_H_

#include <vector>
#include <ostream>
#include "Tile.h"
#include "Rect.h"

class Creature;
class Item;

class DungeonLevel {
	public:
		DungeonLevel();
		DungeonLevel( const DungeonLevel & other );
		virtual ~DungeonLevel();
		virtual void draw( std::ostream & os ) const;
		std::vector<std::vector<char>> draw() const;
		virtual void spawnCreature( Creature * creature );
		virtual void spawnItem( Item * item );
		virtual void spawnItem( Item * item, int x, int y );
		virtual void setTiles( const std::vector<std::vector<Tile>> & tiles );
		virtual const std::vector<std::vector<Tile>> & getTiles() const;
		virtual std::vector<std::vector<Tile>> & getTiles();
		virtual void setLevel( int level );
		virtual int getLevel() const;
		virtual const std::vector<Rect<int>> & getRooms() const;
		virtual void setCreatures( const std::vector<Creature*> & creatures );
		virtual const std::vector<Creature*> & getCreatures() const;
		virtual std::vector<Creature*> & getCreatures();
		static const int WIDTH = 79;
		static const int HEIGHT = 20;
	private:
		std::vector<std::vector<Tile>> mTiles;
		std::vector<Creature*> mCreatures;
		int mLevel;
		std::vector<Rect<int>> mRooms;
};

#endif
