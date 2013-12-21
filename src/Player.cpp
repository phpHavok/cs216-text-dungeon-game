#include "inc/Player.h"
#include <cstdlib>
#include <iostream>

Player::Player() {}

Player::~Player() {}

void Player::dumpObject() {
	std::cout << "Player:" << std::endl;
	dumpObjectData();
}

void Player::dumpObjectData() {
	Character::dumpObjectData();
}

void Player::writeFragment( std::ostream & output ) {
	output << "\t<Player>" << std::endl;
	writeDataAsFragment( output );
	output << "\t</Player>" << std::endl;
}

void Player::writeDataAsFragment( std::ostream & output ) {
	Character::writeDataAsFragment( output );
}

void Player::setElementData( std::string element, std::string value ) {
	Character::setElementData( element, value );
}
