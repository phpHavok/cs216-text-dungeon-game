#ifndef TILE_H_
#define TILE_H_

#include <ostream>
#include <vector>
#include "Character.h"
#include "Item.h"

class Tile {
	public:
		Tile();
		Tile( char symbol );
		virtual ~Tile();
		virtual void setSymbol( char symbol );
		virtual char getSymbol() const;
		virtual void setCharacter( Character * character );
		virtual Character * getCharacter() const;
		virtual void setItems( const std::vector<Item*> & items );
		virtual const std::vector<Item*> & getItems() const;
		virtual std::vector<Item*> & getItems();
		virtual void draw( std::ostream & os ) const;
		virtual char draw() const;
	private:
		char mSymbol;
		Character * mCharacter;
		std::vector<Item*> mItems;
};

#endif
