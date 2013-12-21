#include "inc/Entity.h"
#include <string>
#include <iostream>
#include <map>
#include <functional>
#include "inc/Item.h"
#include "inc/Weapon.h"
#include "inc/Armor.h"

Entity::Entity()
	: mName( "Entity" ), mSymbol( '?' ), mX( 0 ), mY( 0 ), mWeapon( NULL ), mArmor( NULL ) {}

Entity::~Entity() {}

void Entity::setName( const std::string & name ) {
	mName = name;
}

const std::string & Entity::getName() const {
	return mName;
}

void Entity::setSymbol( char symbol ) {
	mSymbol = symbol;
}

char Entity::getSymbol() const {
	return mSymbol;
}
void Entity::setX( int x ) {
	mX = x;
}

int Entity::getX() const {
	return mX;
}

void Entity::setY( int y ) {
	mY = y;
}

int Entity::getY() const {
	return mY;
}

void Entity::setItems( const std::vector<Item*> & items ) {
	mItems = items;
}

const std::vector<Item*> & Entity::getItems() const {
	return mItems;
}

std::vector<Item*> & Entity::getItems() {
	return mItems;
}

void Entity::setWeapon( Weapon * weapon ) {
	mWeapon = weapon;
}

Weapon * Entity::getWeapon() const {
	return mWeapon;
}

void Entity::setArmor( Armor * armor ) {
	mArmor = armor;
}

Armor * Entity::getArmor() const {
	return mArmor;
}

void Entity::dumpObject() {
	std::cout << "Entity:" << std::endl;
	dumpObjectData();
}

void Entity::dumpObjectData() {
	std::cout << "\tname: " << getName() << std::endl;
	std::cout << "\tsymbol: " << getSymbol() << std::endl;
}

void Entity::writeFragment( std::ostream & output ) {
	output << "\t<Entity>" << std::endl;
	writeDataAsFragment( output );
	output << "\t</Entity>" << std::endl;
}

void Entity::writeDataAsFragment( std::ostream & output ) {
	output << "\t\t<name>" << getName() << "</name>" << std::endl;
	output << "\t\t<symbol>" << getSymbol() << "</symbol>" << std::endl;
}

void Entity::setElementData( std::string element, std::string value ) {
	if ( "name" == element ) {
		mName = value;
	} else if ( "symbol" == element ) {
		mSymbol = value.c_str()[0];
	}
}
