#include "inc/functions.h"
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <functional>
#include <limits>
#include "inc/XMLSerializable.h"

static void parseXMLTree( std::ifstream & filestream, XMLSerializable * parentNode, std::map<std::string, std::function<XMLSerializable*()>> & constructorMap, std::vector<XMLSerializable*> & entityList );

void parseXMLTree( std::ifstream & filestream, XMLSerializable * parentNode, std::map<std::string, std::function<XMLSerializable*()>> & constructorMap, std::vector<XMLSerializable*> & entityList ) {
	std::string nodeName;
	std::string nodeValue;

	while ( ! filestream.eof() ) {
		getline( filestream, nodeName, '>' );

		// If this isn't an empty element, parse deeper.
		if ( '/' != nodeName.back() ) {
			getline( filestream, nodeValue, '<' );

			if ( '/' != filestream.peek() ) {
				XMLSerializable * currentNode = NULL;

				if ( NULL == parentNode && 0 != constructorMap.count( nodeName ) )
					currentNode = constructorMap[ nodeName ]();

				parseXMLTree( filestream, currentNode, constructorMap, entityList );

				if ( NULL != currentNode )
					entityList.push_back( currentNode );
			} else {
				if ( NULL != parentNode )
					parentNode->setElementData( nodeName, nodeValue );
			}

			filestream.ignore( std::numeric_limits<std::streamsize>::max(), '>' );
		}

		filestream.ignore( std::numeric_limits<std::streamsize>::max(), '<' );
		
		if ( '/' == filestream.peek() )
			return;
	}
}

void parseXML( std::string filename, std::map<std::string, std::function<XMLSerializable*()>> & constructorMap, std::vector<XMLSerializable*> & entityList ) {
	std::ifstream filestream( filename );
	if ( filestream.fail() )
		throw "Could not open XML file for parsing.";
	
	std::string xmlTemp;

	// Parse XML header.
	getline( filestream, xmlTemp, ' ' );
	if ( "<?xml" != xmlTemp )
		throw "File contains invalid XML.";
	
	filestream.ignore( std::numeric_limits<std::streamsize>::max(), '?' );

	if ( '>' != filestream.peek() )
		throw "File contains invalid XML.";
	
	filestream.ignore( std::numeric_limits<std::streamsize>::max(), '<' );

	parseXMLTree( filestream, NULL, constructorMap, entityList );

	filestream.close();
}

void writeXML( std::string filename, std::vector<XMLSerializable*> entityList ) {
	std::ofstream filestream( filename );
	if ( filestream.fail() )
		throw "Could not open XML file for writing.";
	
	filestream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
	filestream << "<World>" << std::endl;

	for ( unsigned int i = 0; i < entityList.size(); ++i ) {
		entityList[ i ]->writeFragment( filestream );
	}

	filestream << "</World>";

	filestream.close();
}
