#ifndef WEAPON_H_
#define WEAPON_H_

#include "Equipment.h"

class Weapon : public Equipment {
	public:
		Weapon();
		virtual ~Weapon();
		virtual void setAttackBonus( int attackBonus );
		virtual int getAttackBonus() const;
		// XMLSerializable overrides.
		virtual void dumpObject();
		virtual void dumpObjectData();
		virtual void writeFragment( std::ostream & output );
		virtual void writeDataAsFragment( std::ostream & output );
		virtual void setElementData( std::string element, std::string value );
	private:
		int mAttackBonus;
};

#endif
