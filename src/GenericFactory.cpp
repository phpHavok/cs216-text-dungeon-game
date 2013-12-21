#include "inc/GenericFactory.h"
#include <functional>
#include <map>
#include <string>
#include "inc/functions.h"
#include "inc/XMLSerializable.h"
#include "inc/Creature.h"
#include "inc/Item.h"
#include "inc/Consumable.h"
#include "inc/Equipment.h"
#include "inc/Armor.h"
#include "inc/Weapon.h"
#include "inc/Random.h"

GenericFactory & GenericFactory::getInstance() {
	static GenericFactory factory;
	return factory;
}

GenericFactory::GenericFactory() {
	std::map<std::string, std::function<XMLSerializable*()>> constructorMap;
	
	// Load in creatures.
	constructorMap[ "Creature" ] = []() { return new Creature; };

	std::vector<XMLSerializable*> creatures;
	parseXML( "res/creatures.xml", constructorMap, creatures );

	for ( auto creature : creatures ) {
		Creature * temp_creature = dynamic_cast<Creature*>( creature );

		if ( NULL != temp_creature )
			mCreatures.push_back( temp_creature );
	}

	// Load in items.
	constructorMap[ "Item" ] = []() { return new Item; };
	constructorMap[ "Consumable" ] = []() { return new Consumable; };
	constructorMap[ "Equipment" ] = []() { return new Equipment; };
	constructorMap[ "Armor" ] = []() { return new Armor; };
	constructorMap[ "Weapon" ] = []() { return new Weapon; };

	std::vector<XMLSerializable*> items;
	parseXML( "res/items.xml", constructorMap, items );

	for ( auto item : items ) {
		Item * temp_item = dynamic_cast<Item*>( item );

		if ( NULL != temp_item )
			mItems.push_back( temp_item );
	}
}

GenericFactory::~GenericFactory() {
	for ( auto creature : mCreatures ) {
		delete creature;
	}

	for ( auto item : mItems ) {
		delete item;
	}
}

Creature * GenericFactory::generateCreature( int maxLevel ) {
	std::vector<Creature*> usableCreatures;

	for ( auto creature : mCreatures ) {
		if ( creature->getLevel() <= maxLevel )
			usableCreatures.push_back( creature );
	}

	return new Creature( *usableCreatures[ Random::getInstance().rand( 0, usableCreatures.size() - 1 ) ] );
}

Item * GenericFactory::generateItem( int maxLevel ) {
	std::vector<Item*> usableItems;

	for ( auto item : mItems ) {
		if ( item->getLevel() <= maxLevel )
			usableItems.push_back( item );
	}

	Item * item = usableItems[ Random::getInstance().rand( 0, usableItems.size() - 1 ) ];

	if ( NULL != dynamic_cast<Weapon*>( item ) )
		return new Weapon( *dynamic_cast<Weapon*>( item ) );
	else if ( NULL != dynamic_cast<Armor*>( item ) )
		return new Armor( *dynamic_cast<Armor*>( item ) );
	else if ( NULL != dynamic_cast<Consumable*>( item ) )
		return new Consumable( *dynamic_cast<Consumable*>( item ) );
	else
		return new Item( *item );
}
