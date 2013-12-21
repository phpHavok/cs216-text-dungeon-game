#ifndef ITEM_H_
#define ITEM_H_

#include "Entity.h"

class Item : public Entity {
	public:
		Item();
		virtual ~Item();
		virtual void setValue( int value );
		virtual int getValue() const;
		virtual void setWeight( int weight );
		virtual int getWeight() const;
		virtual void setLevel( int level );
		virtual int getLevel() const;
		// XMLSerializable overrides.
		virtual void dumpObject();
		virtual void dumpObjectData();
		virtual void writeFragment( std::ostream & output );
		virtual void writeDataAsFragment( std::ostream & output );
		virtual void setElementData( std::string element, std::string value );
	private:
		int mValue;
		int mWeight;
		int mLevel;
};

#endif
