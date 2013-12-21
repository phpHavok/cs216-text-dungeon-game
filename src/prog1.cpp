#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <functional>
#include <limits>
#include "inc/XMLSerializable.h"
#include "inc/Item.h"
#include "inc/Armor.h"
#include "inc/Weapon.h"
#include "inc/Creature.h"
using namespace std;

void parseXMLTree( ifstream & filestream, int currentDepth, int maxDepth, XMLSerializable * parentNode, map<string, function<XMLSerializable*()>> & constructorMap, vector<XMLSerializable*> & entityList ) {
	string nodeName;
	string nodeValue;

	while ( ! filestream.eof() ) {
		getline( filestream, nodeName, '>' );

		// If this isn't an empty element, parse deeper.
		if ( '/' != nodeName.back() ) {
			getline( filestream, nodeValue, '<' );

			if ( '/' != filestream.peek() ) {
				XMLSerializable * currentNode = NULL;

				if ( NULL == parentNode && 0 != constructorMap.count( nodeName ) )
					currentNode = constructorMap[ nodeName ]();

				parseXMLTree( filestream, currentDepth + 1, maxDepth, currentNode, constructorMap, entityList );

				if ( NULL != currentNode )
					entityList.push_back( currentNode );
			} else {
				if ( NULL != parentNode )
					parentNode->setElementData( nodeName, nodeValue );
			}

			filestream.ignore( numeric_limits<streamsize>::max(), '>' );
		}

		filestream.ignore( numeric_limits<streamsize>::max(), '<' );
		
		if ( '/' == filestream.peek() )
			return;
	}
}

void parseXML( string filename, int maxDepth, map<string, function<XMLSerializable*()>> & constructorMap, vector<XMLSerializable*> & entityList ) {
	ifstream filestream( filename );
	if ( filestream.fail() )
		throw "Could not open XML file for parsing.";
	
	string xmlTemp;

	// Parse XML header.
	getline( filestream, xmlTemp, ' ' );
	if ( "<?xml" != xmlTemp )
		throw "File contains invalid XML.";
	
	filestream.ignore( numeric_limits<streamsize>::max(), '?' );

	if ( '>' != filestream.peek() )
		throw "File contains invalid XML.";
	
	filestream.ignore( numeric_limits<streamsize>::max(), '<' );

	parseXMLTree( filestream, 0, maxDepth, NULL, constructorMap, entityList );

	filestream.close();
}

void writeXML( string filename, vector<XMLSerializable*> entityList ) {
	ofstream filestream( filename );
	if ( filestream.fail() )
		throw "Could not open XML file for writing.";
	
	filestream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
	filestream << "<World>" << endl;

	for ( unsigned int i = 0; i < entityList.size(); ++i ) {
		entityList[ i ]->writeFragment( filestream );
	}

	filestream << "</World>";

	filestream.close();
}

int main( int argc, char * argv[] ) {
	map<string,function<XMLSerializable*()>> constructorMap;
	constructorMap[ "Item" ] = [](){ return new Item; };
	constructorMap[ "Armor" ] = [](){ return new Armor; };
	constructorMap[ "Weapon" ] = [](){ return new Weapon; };
	constructorMap[ "Creature" ] = [](){ return new Creature; };

	vector<XMLSerializable*> entityList;
	
	string inputFilename;
	string outputFilename;

	// Read in and deserialize the XML data.
	cout << "Enter filename to read XML data from." << endl << ">";
	getline( cin, inputFilename );
	try {
		parseXML( inputFilename, 2, constructorMap, entityList );
	} catch ( const char * e ) {
		cout << e << endl;
		return 1;
	}

	// Display the XML data to console.
	cout << "Now dumping objects." << endl;
	for ( unsigned int i = 0; i < entityList.size(); ++i ) {
		entityList[ i ]->dumpObject();
	}

	// Serialize and write the XML data back to file.
	cout << "Enter filename to write XML data to." << endl << ">";
	getline( cin, outputFilename );
	try {
		writeXML( outputFilename, entityList );
	} catch ( const char * e ) {
		cout << e << endl;
		return 1;
	}

	return 0;
}
