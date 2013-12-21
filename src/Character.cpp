#include "inc/Character.h"
#include <cstdlib>
#include <iostream>
#include "inc/Weapon.h"
#include "inc/Armor.h"
#include "inc/Game.h"

Character::Character()
	: mMaxHealth( 10 ), mHealth( 10 ), mExperience( 0 ), mLevel( 1 ), mBaseAttack( 1 ), mBaseDefense( 0 ) {}

Character::~Character() {}

void Character::setMaxHealth( int maxHealth ) {
	mMaxHealth = maxHealth;
}

int Character::getMaxHealth() const {
	return mMaxHealth;
}

void Character::setHealth( int health ) {
	mHealth = ( health > mMaxHealth ) ? mMaxHealth : health;
}

int Character::getHealth() const {
	return mHealth;
}

void Character::setExperience( int experience ) {
	mExperience = experience;
}

int Character::getExperience() const {
	return mExperience;
}

void Character::setLevel( int level ) {
	mLevel = level;
}

int Character::getLevel() const {
	return mLevel;
}

void Character::setBaseAttack( int baseAttack ) {
	mBaseAttack = baseAttack;

}

int Character::getBaseAttack() const {
	return mBaseAttack;

}

void Character::setBaseDefense( int baseDefense ) {
	mBaseDefense = baseDefense;
}

int Character::getBaseDefense() const {
	return mBaseDefense;
}

void Character::attack( Character & target ) {
	int damage = getBaseAttack();

	Weapon * weapon = getWeapon();
	if ( NULL != weapon )
		damage += weapon->getAttackBonus();

	Armor * target_armor = target.getArmor();
	if ( NULL != target_armor )
		damage -= target_armor->getDefenseBonus();
	
	damage -= target.getBaseDefense();

	if ( damage < 1 )
		damage = 1;
	
	Game & game = Game::getInstance();

	game.os() << getName() << " attacks " << target.getName() << " for " << damage << " damage." << std::endl;
	target.setHealth( target.getHealth() - damage );

	if ( target.getHealth() <= 0 )
		game.os() << target.getName() << " is dead." << std::endl;
	else
		game.os() << target.getName() << " has " << target.getHealth() << "/" << target.getMaxHealth() << " health remaining." << std::endl;
}

void Character::dumpObject() {
	std::cout << "Character:" << std::endl;
	dumpObjectData();
}

void Character::dumpObjectData() {
	Entity::dumpObjectData();
	std::cout << "\tmaxHealth: " << getMaxHealth() << std::endl;
	std::cout << "\texperience: " << getExperience() << std::endl;
	std::cout << "\tlevel: " << getLevel() << std::endl;
	std::cout << "\tbaseAttack: " << getBaseAttack() << std::endl;
	std::cout << "\tbaseDefense: " << getBaseDefense() << std::endl;
}

void Character::writeFragment( std::ostream & output ) {
	output << "\t<Character>" << std::endl;
	writeDataAsFragment( output );
	output << "\t</Character>" << std::endl;
}

void Character::writeDataAsFragment( std::ostream & output ) {
	Entity::writeDataAsFragment( output );
	output << "\t\t<maxHealth>" << getMaxHealth() << "</maxHealth>" << std::endl;
	output << "\t\t<experience>" << getExperience() << "</experience>" << std::endl;
	output << "\t\t<level>" << getLevel() << "</level>" << std::endl;
	output << "\t\t<baseAttack>" << getBaseAttack() << "</baseAttack>" << std::endl;
	output << "\t\t<baseDefense>" << getBaseDefense() << "</baseDefense>" << std::endl;
}

void Character::setElementData( std::string element, std::string value ) {
	Entity::setElementData( element, value );

	if ( "maxHealth" == element ) {
		mMaxHealth = atoi( value.c_str() );
		mHealth = mMaxHealth;
	} else if ( "experience" == element ) {
		mExperience = atoi( value.c_str() );
	} else if ( "level" == element ) {
		mLevel = atoi( value.c_str() );
	} else if ( "baseAttack" == element ) {
		mBaseAttack = atoi( value.c_str() );
	} else if ( "baseDefense" == element ) {
		mBaseDefense = atoi( value.c_str() );
	}
}
