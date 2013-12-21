#include "inc/Equipment.h"
#include <iostream>

Equipment::Equipment() {}

Equipment::~Equipment() {}

void Equipment::dumpObject() {
	std::cout << "Equipment:" << std::endl;
	dumpObjectData();
}

void Equipment::dumpObjectData() {
	Item::dumpObjectData();
}

void Equipment::writeFragment( std::ostream & output ) {
	output << "\t<Equipment>" << std::endl;
	writeDataAsFragment( output );
	output << "\t</Equipment>" << std::endl;
}

void Equipment::writeDataAsFragment( std::ostream & output ) {
	Item::writeDataAsFragment( output );
}

void Equipment::setElementData( std::string element, std::string value ) {
	Item::setElementData( element, value );
}
