#ifndef EQUIPMENT_H_
#define EQUIPMENT_H_

#include "Item.h"

class Equipment : public Item {
	public:
		Equipment();
		virtual ~Equipment();
		// XMLSerializable overrides.
		virtual void dumpObject();
		virtual void dumpObjectData();
		virtual void writeFragment( std::ostream & output );
		virtual void writeDataAsFragment( std::ostream & output );
		virtual void setElementData( std::string element, std::string value );
};

#endif
