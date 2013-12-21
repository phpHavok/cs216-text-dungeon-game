#ifndef CONSUMABLE_H_
#define CONSUMABLE_H_

#include "Item.h"
#include "Character.h"

class Consumable : public Item {
	public:
		Consumable();
		virtual ~Consumable();
		virtual void setHealthBonus( int healthBonus );
		virtual int getHealthBonus() const;
		virtual void use( Character & user );
		// XMLSerializable overrides.
		virtual void dumpObject();
		virtual void dumpObjectData();
		virtual void writeFragment( std::ostream & output );
		virtual void writeDataAsFragment( std::ostream & output );
		virtual void setElementData( std::string element, std::string value );
	private:
		int mHealthBonus;
};

#endif
