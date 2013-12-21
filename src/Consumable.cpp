#include "inc/Consumable.h"
#include <cstdlib>
#include <iostream>
#include "inc/Game.h"

Consumable::Consumable()
	: mHealthBonus( 10 ) {}

Consumable::~Consumable() {}

void Consumable::setHealthBonus( int healthBonus ) {
	mHealthBonus = healthBonus;
}

int Consumable::getHealthBonus() const {
	return mHealthBonus;
}

void Consumable::use( Character & user ) {
	Game & game = Game::getInstance();

	user.setHealth( user.getHealth() + getHealthBonus() );
	game.os() << user.getName() << " +" << getHealthBonus() << " health." << std::endl;
	game.os() << user.getName() << " now has " << user.getHealth() << "/" << user.getMaxHealth() << " health." << std::endl;
}

void Consumable::dumpObject() {
	std::cout << "Consumable:" << std::endl;
	dumpObjectData();
}

void Consumable::dumpObjectData() {
	Item::dumpObjectData();
	std::cout << "\thealthBonus: " << getHealthBonus() << std::endl;
}

void Consumable::writeFragment( std::ostream & output ) {
	output << "\t<Consumable>" << std::endl;
	writeDataAsFragment( output );
	output << "\t</Consumable>" << std::endl;
}

void Consumable::writeDataAsFragment( std::ostream & output ) {
	Item::writeDataAsFragment( output );
	output << "\t\t<healthBonus>" << getHealthBonus() << "</healthBonus>" << std::endl;
}

void Consumable::setElementData( std::string element, std::string value ) {
	Item::setElementData( element, value );

	if ( "healthBonus" == element ) {
		mHealthBonus = atoi( value.c_str() );
	}
}
