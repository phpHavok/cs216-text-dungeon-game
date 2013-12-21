#include "inc/Item.h"
#include <cstdlib>
#include <iostream>

Item::Item()
	: mValue( 0 ), mWeight( 0 ), mLevel( 1 ) {}

Item::~Item() {}

void Item::setValue( int value ) {
	mValue = value;
}

int Item::getValue() const {
	return mValue;
}

void Item::setWeight( int weight ) {
	mWeight = weight;
}

int Item::getWeight() const {
	return mWeight;
}

void Item::setLevel( int level ) {
	mLevel = level;
}

int Item::getLevel() const {
	return mLevel;
}

void Item::dumpObject() {
	std::cout << "Item:" << std::endl;
	dumpObjectData();
}

void Item::dumpObjectData() {
	Entity::dumpObjectData();
	std::cout << "\tvalue: " << getValue() << std::endl;
	std::cout << "\tweight: " << getWeight() << std::endl;
	std::cout << "\tlevel: " << getLevel() << std::endl;
}

void Item::writeFragment( std::ostream & output ) {
	output << "\t<Item>" << std::endl;
	writeDataAsFragment( output );
	output << "\t</Item>" << std::endl;
}

void Item::writeDataAsFragment( std::ostream & output ) {
	Entity::writeDataAsFragment( output );
	output << "\t\t<value>" << getValue() << "</value>" << std::endl;
	output << "\t\t<weight>" << getWeight() << "</weight>" << std::endl;
	output << "\t\t<level>" << getLevel() << "</level>" << std::endl;
}

void Item::setElementData( std::string element, std::string value ) {
	Entity::setElementData( element, value );

	if ( "value" == element ) {
		mValue = atoi( value.c_str() );
	} else if ( "weight" == element ) {
		mWeight = atoi( value.c_str() );
	} else if ( "level" == element ) {
		mLevel = atoi( value.c_str() );
	}
}
