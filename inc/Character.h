#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "Entity.h"

class Character : public Entity {
	public:
		Character();
		virtual ~Character();
		virtual void setMaxHealth( int maxHealth );
		virtual int getMaxHealth() const;
		virtual void setHealth( int health );
		virtual int getHealth() const;
		virtual void setExperience( int experience );
		virtual int getExperience() const;
		virtual void setLevel( int level );
		virtual int getLevel() const;
		virtual void setBaseAttack( int baseAttack );
		virtual int getBaseAttack() const;
		virtual void setBaseDefense( int baseDefense );
		virtual int getBaseDefense() const;
		virtual void attack( Character & target );
		// XMLSerializable overrides.
		virtual void dumpObject();
		virtual void dumpObjectData();
		virtual void writeFragment( std::ostream & output );
		virtual void writeDataAsFragment( std::ostream & output );
		virtual void setElementData( std::string element, std::string value );
	private:
		int mMaxHealth;
		int mHealth;
		int mExperience;
		int mLevel;
		int mBaseAttack;
		int mBaseDefense;
};

#endif
