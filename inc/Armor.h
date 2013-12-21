#ifndef ARMOR_H_
#define ARMOR_H_

#include "Equipment.h"

class Armor : public Equipment {
	public:
		Armor();
		virtual ~Armor();
		virtual void setDefenseBonus( int defenseBonus );
		virtual int getDefenseBonus() const;
		// XMLSerializable overrides.
		virtual void dumpObject();
		virtual void dumpObjectData();
		virtual void writeFragment( std::ostream & output );
		virtual void writeDataAsFragment( std::ostream & output );
		virtual void setElementData( std::string element, std::string value );
	private:
		int mDefenseBonus;
};

#endif
