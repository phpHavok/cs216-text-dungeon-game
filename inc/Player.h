#ifndef PLAYER_H_
#define PLAYER_H_

#include "Character.h"

class Player : public Character {
	public:
		Player();
		virtual ~Player();
		// XMLSerializable overrides.
		virtual void dumpObject();
		virtual void dumpObjectData();
		virtual void writeFragment( std::ostream & output );
		virtual void writeDataAsFragment( std::ostream & output );
		virtual void setElementData( std::string element, std::string value );
};

#endif
