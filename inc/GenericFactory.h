#ifndef GENERICFACTORY_H_
#define GENERICFACTORY_H_

#include <vector>

class Creature;
class Item;

class GenericFactory {
	public:	
		static GenericFactory & getInstance();
		virtual ~GenericFactory();
		virtual Creature * generateCreature( int maxLevel );
		virtual Item * generateItem( int maxLevel );
	private:
		GenericFactory();
		std::vector<Creature*> mCreatures;
		std::vector<Item*> mItems;
};

#endif
