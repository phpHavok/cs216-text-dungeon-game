#include <string>
#include <vector>
#include <map>
#include <functional>
#include "XMLSerializable.h"

void parseXML( std::string filename, std::map<std::string, std::function<XMLSerializable*()>> & constructorMap, std::vector<XMLSerializable*> & entityList );
void writeXML( std::string filename, std::vector<XMLSerializable*> entityList );
