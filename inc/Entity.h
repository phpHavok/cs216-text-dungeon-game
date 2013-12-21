#ifndef ENTITY_H_
#define ENTITY_H_

#include "XMLSerializable.h"
#include <string>
#include <vector>

class Item;
class Weapon;
class Armor;

class Entity : public XMLSerializable {
	public:
		Entity();
		virtual ~Entity();
		virtual void setName( const std::string & name );
		virtual const std::string & getName() const;
		virtual void setSymbol( char symbol );
		virtual char getSymbol() const;
		virtual void setX( int x );
		virtual int getX() const;
		virtual void setY( int y );
		virtual int getY() const;
		virtual void setItems( const std::vector<Item*> & items );
		virtual const std::vector<Item*> & getItems() const;
		virtual std::vector<Item*> & getItems();
		virtual void setWeapon( Weapon * weapon );
		virtual Weapon * getWeapon() const;
		virtual void setArmor( Armor * armor );
		virtual Armor * getArmor() const;
		// XMLSerializable overrides.
		virtual void dumpObject();
		virtual void dumpObjectData();
		virtual void writeFragment( std::ostream & output );
		virtual void writeDataAsFragment( std::ostream & output );
		virtual void setElementData( std::string element, std::string value );
	private:
		std::string mName;
		char mSymbol;
		int mX;
		int mY;
		std::vector<Item*> mItems;
		Weapon * mWeapon;
		Armor * mArmor;
};

#endif
