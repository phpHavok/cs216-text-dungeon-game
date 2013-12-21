#include <iostream>
#include <map>
#include <string>
#include <functional>
#include "inc/Item.h"
#include "inc/Armor.h"
#include "inc/Weapon.h"
#include "inc/Creature.h"
using namespace std;

int main( int argc, char * argv[] ) {
	map<string,function<XMLSerializable*()>> mapConstructor;
	mapConstructor[ "Item" ] = [](){ return new Item; };
	mapConstructor[ "Armor" ] = [](){ return new Armor; };
	mapConstructor[ "Weapon" ] = [](){ return new Weapon; };
	mapConstructor[ "Creature" ] = [](){ return new Creature; };

	cout << "Enter the name of a class to construct." << endl << ">";

	string inputString;
	getline( cin, inputString );

	if ( mapConstructor.end() != mapConstructor.find( inputString ) ) {
		XMLSerializable * constructedClass = mapConstructor[ inputString ]();
		cout << "Class found and constructed." << endl;
	} else {
		cout << "Sorry, that class is not in our map." << endl;
	}

	return 0;
}
