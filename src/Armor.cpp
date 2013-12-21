#include "inc/Armor.h"
#include <cstdlib>
#include <iostream>

Armor::Armor()
	: mDefenseBonus( 0 ) {}

Armor::~Armor() {}

void Armor::setDefenseBonus( int defenseBonus ) {
	mDefenseBonus = defenseBonus;
}

int Armor::getDefenseBonus() const {
	return mDefenseBonus;
}

void Armor::dumpObject() {
	std::cout << "Armor:" << std::endl;
	dumpObjectData();
}

void Armor::dumpObjectData() {
	Equipment::dumpObjectData();
	std::cout << "\tdefenseBonus: " << getDefenseBonus() << std::endl;
}

void Armor::writeFragment( std::ostream & output ) {
	output << "\t<Armor>" << std::endl;
	writeDataAsFragment( output );
	output << "\t</Armor>" << std::endl;
}

void Armor::writeDataAsFragment( std::ostream & output ) {
	Equipment::writeDataAsFragment( output );
	output << "\t\t<defenseBonus>" << getDefenseBonus() << "</defenseBonus>" << std::endl;
}

void Armor::setElementData( std::string element, std::string value ) {
	Equipment::setElementData( element, value );

	if ( "defenseBonus" == element ) {
		mDefenseBonus = atoi( value.c_str() );
	}
}
