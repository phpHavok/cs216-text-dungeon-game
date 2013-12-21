#include "inc/Tile.h"

Tile::Tile()
	: mSymbol( ' ' ), mCharacter( NULL )
{}

Tile::~Tile() {}

void Tile::setSymbol( char symbol ) {
	mSymbol = symbol;
}

char Tile::getSymbol() const {
	return mSymbol;
}

void Tile::setCharacter( Character * character ) {
	mCharacter = character;
}

Character * Tile::getCharacter() const {
	return mCharacter;
}

void Tile::setItems( const std::vector<Item*> & items ) {
	mItems = items;
}

const std::vector<Item*> & Tile::getItems() const {
	return mItems;
}

std::vector<Item*> & Tile::getItems() {
	return mItems;
}

void Tile::draw( std::ostream & os ) const {
	os << draw();
}

char Tile::draw() const {
	if ( NULL != mCharacter )
		return mCharacter->getSymbol();
	else if ( ! mItems.empty() )
		return mItems[ 0 ]->getSymbol();
	else
		return mSymbol;
}
