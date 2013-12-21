#include "inc/Weapon.h"
#include <cstdlib>
#include <iostream>

Weapon::Weapon()
	: mAttackBonus( 0 ) {}

Weapon::~Weapon() {}

void Weapon::setAttackBonus( int attackBonus ) {
	mAttackBonus = attackBonus;
}

int Weapon::getAttackBonus() const {
	return mAttackBonus;
}

void Weapon::dumpObject() {
	std::cout << "Weapon:" << std::endl;
	dumpObjectData();
}

void Weapon::dumpObjectData() {
	Equipment::dumpObjectData();
	std::cout << "\tattackBonus: " << getAttackBonus() << std::endl;
}

void Weapon::writeFragment( std::ostream & output ) {
	output << "\t<Weapon>" << std::endl;
	writeDataAsFragment( output );
	output << "\t</Weapon>" << std::endl;
}

void Weapon::writeDataAsFragment( std::ostream & output ) {
	Equipment::writeDataAsFragment( output );
	output << "\t\t<attackBonus>" << getAttackBonus() << "</attackBonus>" << std::endl;
}

void Weapon::setElementData( std::string element, std::string value ) {
	Equipment::setElementData( element, value );

	if ( "attackBonus" == element ) {
		mAttackBonus = atoi( value.c_str() );
	}
}
