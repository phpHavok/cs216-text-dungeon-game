#ifndef RANDOM_H_
#define RANDOM_H_

#include <random>

class Random {
	public:
		static Random & getInstance();
		virtual ~Random();
		virtual void seed( unsigned int value );
		virtual unsigned int rand( unsigned int min, unsigned int max );
	private:
		Random();
		std::mt19937 mGenerator;
};

#endif
