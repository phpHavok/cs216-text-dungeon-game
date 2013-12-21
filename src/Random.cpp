#include "inc/Random.h"
#include <ctime>

Random & Random::getInstance() {
	static Random instance;
	return instance;
}

Random::Random() {
	seed( static_cast<unsigned int>( time( NULL ) ) );
}

Random::~Random() {}

void Random::seed( unsigned int value ) {
	mGenerator.seed( value );
}

unsigned int Random::rand( unsigned int min, unsigned int max ) {
	return min + mGenerator() % ( max - min + 1 );
}
