#include "inc/Game.h"
#include <fstream>
#include <string>
#include <iostream>

int main( int argc, char * argv[] ) {
	std::ifstream ifs( "res/README-light" );

	if ( ifs.fail() ) {
		std::cout << "Could not load readme." << std::endl;
		return 1;
	}

	std::string input_string;

	while ( getline( ifs, input_string ) ) {
		std::cout << input_string << std::endl;
	}

	ifs.close();

	std::cout << std::endl << "When you are ready to begin, press enter." << std::endl;
	getline( std::cin, input_string );
	
	Game::getInstance().gameLoop();
	
	return 0;
}
