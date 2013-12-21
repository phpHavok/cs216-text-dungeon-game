#ifndef CREATURE_H_
#define CREATURE_H_

#include "Character.h"

class Creature : public Character {
	public:
		Creature();
		virtual ~Creature();
		virtual void setHostile( bool hostile );
		virtual bool getHostile() const;
		// XMLSerializable overrides.
		virtual void dumpObject();
		virtual void dumpObjectData();
		virtual void writeFragment( std::ostream & output );
		virtual void writeDataAsFragment( std::ostream & output );
		virtual void setElementData( std::string element, std::string value );
	private:
		bool mHostile;
};

#endif
