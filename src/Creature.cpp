#include "inc/Creature.h"
#include <cstdlib>
#include <iostream>

Creature::Creature()
	: mHostile( true ) {}

Creature::~Creature() {}

void Creature::setHostile( bool hostile ) {
	mHostile = hostile;
}

bool Creature::getHostile() const {
	return mHostile;
}

void Creature::dumpObject() {
	std::cout << "Creature:" << std::endl;
	dumpObjectData();
}

void Creature::dumpObjectData() {
	Character::dumpObjectData();
	std::cout << "\thostile: " << ( getHostile() ? "true" : "false" ) << std::endl;
}

void Creature::writeFragment( std::ostream & output ) {
	output << "\t<Creature>" << std::endl;
	writeDataAsFragment( output );
	output << "\t</Creature>" << std::endl;
}

void Creature::writeDataAsFragment( std::ostream & output ) {
	Character::writeDataAsFragment( output );
	output << "\t\t<hostile>" << ( getHostile() ? "true" : "false" ) << "</hostile>" << std::endl;
}

void Creature::setElementData( std::string element, std::string value ) {
	Character::setElementData( element, value );

	if ( "hostile" == element ) {
		mHostile = ( "true" == value );
	}
}
