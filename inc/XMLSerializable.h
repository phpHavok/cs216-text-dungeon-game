#ifndef XMLSERIALIZABLE_H_
#define XMLSERIALIZABLE_H_

#include <ostream>

class XMLSerializable {
	public:
		virtual void dumpObject() = 0;
		virtual void dumpObjectData() = 0;
		virtual void writeFragment( std::ostream & output ) = 0;
		virtual void writeDataAsFragment( std::ostream & output ) = 0;
		virtual void setElementData( std::string element, std::string value ) = 0;
};

#endif
