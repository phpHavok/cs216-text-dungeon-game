#include "inc/Game.h"
#include <ctime>
#include <string>
#include <iostream>
#include "inc/DungeonLevel.h"
#include "inc/Creature.h"
#include "inc/Item.h"
#include "inc/Consumable.h"
#include "inc/Weapon.h"
#include "inc/Armor.h"
#include "inc/GenericFactory.h"
#include "inc/Random.h"

Game & Game::getInstance() {
	static Game instance;
	return instance;
}

Game::Game()
	: mCurrentMap( 0 )
{
	// Setup ncurses.
	initscr();
	start_color();
	init_pair( CP_DARK, COLOR_BLACK, COLOR_BLACK );
	cbreak();
	noecho();
	nonl();

	mWindowMap = newwin( DungeonLevel::HEIGHT, DungeonLevel::WIDTH, 0, 0 );
	mWindowInteractive = newwin( DungeonLevel::HEIGHT, 0, 0, DungeonLevel::WIDTH );

	intrflush( mWindowInteractive, FALSE );
	keypad( mWindowInteractive, TRUE );
	scrollok( mWindowInteractive, TRUE );

	// Create the player.
	mPlayer.setName( "Player" );
	mPlayer.setSymbol( 'P' );
	mPlayer.setMaxHealth( 20 );
	mPlayer.setHealth( 20 );
	mPlayer.setLevel( 1 );
	mPlayer.setExperience( 0 );

	// Create the initial map.
	DungeonLevel * map = new DungeonLevel;
	map->setLevel( 1 );
	mMaps.push_back( map );

	movePlayerToUpstairs();
	spawnCreatures();
	spawnItems();
}

Game::~Game() {
	for ( auto map : mMaps )
		delete map;
	
	for ( auto creature : mCreatures )
		delete creature;
	
	for ( auto item : mItems )
		delete item;
	
	// Close ncurses.
	delwin( mWindowMap );
	delwin( mWindowInteractive );

	endwin();
}

void Game::gameLoop() {
	int user_command;

	bool game_over = false;

	time_t timer_ticker;
	time( &timer_ticker );

	while ( ! game_over ) {
		// Do a game phase for random events.
		if ( time( NULL ) - timer_ticker >= 5 ) {
			time( &timer_ticker );

			if ( 0 == Random::getInstance().rand( 0, 5 ) ) {
				// 20% chance to spawn more monsters.
				spawnCreatures();
			}

			// Definiately give the player some health.
			mPlayer.setHealth( mPlayer.getHealth() + 1 );
		}

		// Draw the current map.
		wclear( mWindowMap );

		std::vector<std::vector<char>> tiles_to_draw = mMaps[ mCurrentMap ]->draw();
		for ( unsigned int r = 0; r < tiles_to_draw.size(); ++r ) {
			for ( unsigned int c = 0; c < tiles_to_draw[ r ].size(); ++c ) {
				wmove( mWindowMap, r, c );
				waddch( mWindowMap, tiles_to_draw[ r ][ c ] | playerVisibilityMap( c, r ) );
			}
		}

		wrefresh( mWindowMap );

		// Get user input.
		user_command = wgetch( mWindowInteractive );

		// Process user input.
		switch ( user_command ) {
			case 'W':
			case 'w':
			case KEY_UP:
				// Move player up.
				movePlayer( mPlayer.getX(), mPlayer.getY() - 1 );
				break;

			case 'S':
			case 's':
			case KEY_DOWN:
				// Move player down.
				movePlayer( mPlayer.getX(), mPlayer.getY() + 1 );
				break;

			case 'A':
			case 'a':
			case KEY_LEFT:
				// Move player left.
				movePlayer( mPlayer.getX() - 1, mPlayer.getY() );
				break;

			case 'D':
			case 'd':
			case KEY_RIGHT:
				// Move player right.
				movePlayer( mPlayer.getX() + 1, mPlayer.getY() );
				break;

			case '>':
				// Go down a dungeon level.
				movePlayerDownDungeon();
				break;

			case '<':
				// Go up a dungeon level.
				try {
					movePlayerUpDungeon();
				} catch ( ... ) {
					game_over = true;
				}
				break;

			case 'P':
			case 'p':
				// Pick up an item.
				playerPickUpItem();
				break;

			case 'O':
			case 'o':
				// Drop an item.
				playerDropItem();
				break;

			case 'E':
			case 'e':
				// Equip an item.
				playerEquipItem();
				break;

			case 'U':
			case 'u':
				// Use an item.
				playerUseItem();
				break;

			case 'I':
			case 'i':
				// View inventory.
				playerViewInventory();
				break;

			case 'Q':
			case 'q':
				// Quit the game.
				game_over = true;
				break;

			default:
				break;
		}

		if ( game_over )
			break;

		// Now for the creatures.
		for ( auto creature : mMaps[ mCurrentMap ]->getCreatures() ) {
			int creature_x = creature->getX();
			int creature_y = creature->getY();

			int offset_x = mPlayer.getX() - creature_x;
			int offset_y = mPlayer.getY() - creature_y;

			int dx = 0;
			int dy = 0;

			// Passive creatures or creatures that aren't in your line of sight should move randomly.
			if ( ! creature->getHostile() || static_cast<int>( sqrt( offset_x * offset_x + offset_y * offset_y ) ) > 6 ) {
				if ( 0 == Random::getInstance().rand( 0, 1 ) ) {
					dy = ( 0 == Random::getInstance().rand( 0, 1 ) ) ? 1 : -1;
				} else {
					dx = ( 0 == Random::getInstance().rand( 0, 1 ) ) ? 1 : -1;
				}
			} else {
				// In line of sight.
				if ( 0 == offset_x ) {
					dy = ( offset_y >= 0 ) ? 1 : -1;
				} else if ( 0 == offset_y ) {
					dx = ( offset_x >= 0 ) ? 1 : -1;
				} else {
					if ( 0 == Random::getInstance().rand( 0, 1 ) ) {
						dy = ( offset_y >= 0 ) ? 1 : -1;
					} else {
						dx = ( offset_x >= 0 ) ? 1 : -1;
					}
				}
			}


			std::vector<std::vector<Tile>> & tiles = mMaps[ mCurrentMap ]->getTiles();

			if ( !( creature_y + dy >= 0 && creature_y + dy < tiles.size() && creature_x + dx >= 0 && creature_x + dx < tiles[ creature_y + dy ].size() ) )
				continue;
			
			Tile & tile = tiles[ creature_y + dy ][ creature_x + dx ];

			if ( ' ' == tile.getSymbol() )
				continue;

			if ( NULL != tile.getCharacter() ) {
				if ( &mPlayer == tile.getCharacter() && creature->getHostile() ) {
					creature->attack( mPlayer );

					if ( mPlayer.getHealth() <= 0 ) {
						game_over = true;
						break;
					}
				}
			} else {
				tiles[ creature_y ][ creature_x ].setCharacter( NULL );
				tile.setCharacter( creature );
				creature->setX( creature_x + dx );
				creature->setY( creature_y + dy );
			}
		}

		// Update the interactive window with changed text.
		std::string new_text = mOS.str();
		if ( ! new_text.empty() ) {
			waddstr( mWindowInteractive, new_text.c_str() );
			wrefresh( mWindowInteractive );
			mOS.str( "" );
		}
	}

	if ( mPlayer.getHealth() <= 0 ) {
		mOS << "Game over!" << std::endl;
		mOS << "You have died." << std::endl;
		mOS << "Your final score is: " << mPlayer.getExperience() << std::endl;
	} else if ( mCurrentMap < 0 ) {
		mOS << "Game over!" << std::endl;
		mOS << "You have escaped the dungeon." << std::endl;
		mOS << "Your final score is: " << mPlayer.getExperience() << std::endl;
	} else {
		mOS << "Game over!" << std::endl;
		mOS << "You have quit." << std::endl;
		mOS << "Your final score is: " << mPlayer.getExperience() << std::endl;
	}

	mOS << std::endl << "Press enter to exit." << std::endl;

	// Update the interactive window with changed text (if necessary).
	// We have to do this again here outside of the loop.
	std::string new_text = mOS.str();
	if ( ! new_text.empty() ) {
		waddstr( mWindowInteractive, new_text.c_str() );
		wrefresh( mWindowInteractive );
		mOS.str( "" );
	}

	nocbreak();
	wgetch( mWindowInteractive );
}

std::stringstream & Game::os() {
	return mOS;
}

void Game::movePlayerToDownstairs() {
	std::vector<std::vector<Tile>> & tiles = mMaps[ mCurrentMap ]->getTiles();

	for ( int r = 0; r < tiles.size(); ++r ) {
		for ( int c = 0; c < tiles[ r ].size(); ++c ) {
			if ( '>' == tiles[ r ][ c ].getSymbol() ) {
				mPlayer.setX( c );
				mPlayer.setY( r );
				tiles[ r ][ c ].setCharacter( &mPlayer );
			}
		}
	}
}

void Game::movePlayerToUpstairs() {
	std::vector<std::vector<Tile>> & tiles = mMaps[ mCurrentMap ]->getTiles();

	for ( int r = 0; r < tiles.size(); ++r ) {
		for ( int c = 0; c < tiles[ r ].size(); ++c ) {
			if ( '<' == tiles[ r ][ c ].getSymbol() ) {
				mPlayer.setX( c );
				mPlayer.setY( r );
				tiles[ r ][ c ].setCharacter( &mPlayer );
			}
		}
	}
}

void Game::movePlayer( int x, int y ) {
	std::vector<std::vector<Tile>> & tiles = mMaps[ mCurrentMap ]->getTiles();

	if ( !( y >= 0 && y < tiles.size() && x >= 0 && x < tiles[ y ].size() ) )
		return;
	
	Tile & tile = tiles[ y ][ x ];

	if ( ' ' == tile.getSymbol() )
		return;
	
	Character * character = tile.getCharacter();

	if ( NULL != tile.getCharacter() ) {
		mPlayer.attack( *character );

		if ( character->getHealth() <= 0 ) {
			// Chance of dropping a random item on death.
			if ( 0 == Random::getInstance().rand( 0, 5 ) ) {
				mMaps[ mCurrentMap ]->spawnItem( GenericFactory::getInstance().generateItem( character->getLevel() ), character->getX(), character->getY() );
			}

			mOS << "You gain " << character->getExperience() << " experience." << std::endl;
			mPlayer.setExperience( mPlayer.getExperience() + character->getExperience() );

			// Level up!
			if ( mPlayer.getExperience() >= mPlayer.getLevel() * mPlayer.getLevel() * 100 ) {
				mPlayer.setLevel( mPlayer.getLevel() + 1 );
				mOS << "Congrats on level " << mPlayer.getLevel() << ", " << mPlayer.getName() << std::endl;

				if ( 0 == Random::getInstance().rand( 0, 1 ) ) {
					mPlayer.setBaseAttack( mPlayer.getBaseAttack() + 1 );
					mOS << "You gained +1 base attack." << std::endl;
				} else {
					mPlayer.setBaseDefense( mPlayer.getBaseDefense() + 1 );
					mOS << "You gain +1 base defense." << std::endl;
				}

				// Extra health.
				mPlayer.setMaxHealth( mPlayer.getMaxHealth() + 10 );
				mPlayer.setHealth( mPlayer.getMaxHealth() );
				mOS << "You gained +10 health." << std::endl;
			}

			tile.setCharacter( NULL );

			std::vector<Creature*> & creatures = mMaps[ mCurrentMap ]->getCreatures();

			for ( auto i = creatures.begin(); creatures.end() != i; ++i ) {
				if ( *i == character ) {
					creatures.erase( i );
					break;
				}
			}

			for ( auto i = mCreatures.begin(); mCreatures.end() != i; ++i ) {
				if ( *i == character ) {
					mCreatures.erase( i );
					break;
				}
			}

			delete character;
		}
	} else {
		tile.setCharacter( &mPlayer );
		tiles[ mPlayer.getY() ][ mPlayer.getX() ].setCharacter( NULL );
		mPlayer.setX( x );
		mPlayer.setY( y );
	}
}

void Game::movePlayerDownDungeon() {
	std::vector<std::vector<Tile>> & tiles = mMaps[ mCurrentMap ]->getTiles();

	Tile & tile = tiles[ mPlayer.getY() ][ mPlayer.getX() ];

	if ( '>' != tile.getSymbol() )
		return;

	mCurrentMap++;

	if ( mCurrentMap >= mMaps.size() ) {
		DungeonLevel * map = new DungeonLevel;
		map->setLevel( mCurrentMap + 1 );
		mMaps.push_back( map );

		spawnCreatures();
		spawnItems();
	}

	tile.setCharacter( NULL );
	movePlayerToUpstairs();
}

void Game::movePlayerUpDungeon() {
	std::vector<std::vector<Tile>> & tiles = mMaps[ mCurrentMap ]->getTiles();

	Tile & tile = tiles[ mPlayer.getY() ][ mPlayer.getX() ];

	if ( '<' != tile.getSymbol() )
		return;

	mCurrentMap--;

	if ( mCurrentMap < 0 )
		throw "Dungeon escaped.";
	
	tile.setCharacter( NULL );
	movePlayerToDownstairs();
}

void Game::playerPickUpItem() {
	Tile & tile = mMaps[ mCurrentMap ]->getTiles()[ mPlayer.getY() ][ mPlayer.getX() ];
	std::vector<Item*> & items = tile.getItems();
	std::vector<Item*> & mPlayer_items = mPlayer.getItems();

	if ( items.empty() ) {
		mOS << std::string( "There is nothing here to pick up." ) << std::endl;
		return;
	}
	
	for ( unsigned int i = 0; i < items.size(); ++i ) {
		mPlayer_items.push_back( items[ i ] );
		mOS << "Picked up " << items[ i ]->getName() << "." << std::endl;
	}

	items.clear();
}

void Game::playerDropItem() {
	std::vector<Item*> & items = mPlayer.getItems();

	if ( items.empty() ) {
		mOS << "You are not carrying anything to drop." << std::endl;
		return;
	}

	if ( 1 == items.size() ) {
		mOS << "Dropped " << items[ 0 ]->getName() << "." << std::endl;
		mMaps[ mCurrentMap ]->getTiles()[ mPlayer.getY() ][ mPlayer.getX() ].getItems().push_back( items[ 0 ] );

		if ( NULL != mPlayer.getWeapon() && dynamic_cast<Weapon*>( items[ 0 ] ) == mPlayer.getWeapon() )
			mPlayer.setWeapon( NULL );
		else if ( NULL != mPlayer.getArmor() && dynamic_cast<Armor*>( items[ 0 ] ) == mPlayer.getArmor() )
			mPlayer.setArmor( NULL );

		items.clear();
		return;
	}

	mOS << "Drop which item? (1-" << items.size() << ") ";
	waddstr( mWindowInteractive, mOS.str().c_str() );
	mOS.str( "" );

	nocbreak();
	echo();
	nl();
	char user_input[ 128 ];
	wgetstr( mWindowInteractive, user_input );
	int which_item = atoi( user_input );
	cbreak();
	noecho();
	nonl();

	if ( which_item <= 0 || which_item > items.size() )
		return;
	
	mOS << "Dropped " << items[ which_item - 1 ]->getName() << "." << std::endl;
	mMaps[ mCurrentMap ]->getTiles()[ mPlayer.getY() ][ mPlayer.getX() ].getItems().push_back( items[ which_item - 1 ] );

	if ( NULL != mPlayer.getWeapon() && dynamic_cast<Weapon*>( items[ which_item - 1 ] ) == mPlayer.getWeapon() )
		mPlayer.setWeapon( NULL );
	else if ( NULL != mPlayer.getArmor() && dynamic_cast<Armor*>( items[ which_item - 1 ] ) == mPlayer.getArmor() )
		mPlayer.setArmor( NULL );

	items.erase( items.begin() + ( which_item - 1 ) );
}

void Game::playerEquipItem() {
	const std::vector<Item*> & items = mPlayer.getItems();

	if ( items.empty() ) {
		mOS << "You are not carrying anything to equip." << std::endl;
		return;
	}

	if ( 1 == items.size() ) {
		if ( NULL != dynamic_cast<Weapon*>( items[ 0 ] ) ) {
			mOS << "Equipped " << items[ 0 ]->getName() << "." << std::endl;
			mPlayer.setWeapon( dynamic_cast<Weapon*>( items[ 0 ] ) );
		} else if ( NULL != dynamic_cast<Armor*>( items[ 0 ] ) ) {
			mOS << "Equipped " << items[ 0 ]->getName() << "." << std::endl;
			mPlayer.setArmor( dynamic_cast<Armor*>( items[ 0 ] ) );
		} else {
			mOS << "You are not carrying anything equippable." << std::endl;
		}

		return;
	}

	mOS << "Equip which item? (1-" << items.size() << ") ";
	waddstr( mWindowInteractive, mOS.str().c_str() );
	mOS.str( "" );

	nocbreak();
	echo();
	nl();
	char user_input[ 128 ];
	wgetstr( mWindowInteractive, user_input );
	int which_item = atoi( user_input );
	cbreak();
	noecho();
	nonl();

	if ( which_item <= 0 || which_item > items.size() )
		return;
	
	if ( NULL != dynamic_cast<Weapon*>( items[ which_item - 1 ] ) ) {
		mOS << "Equipped " << items[ which_item - 1 ]->getName() << "." << std::endl;
		mPlayer.setWeapon( dynamic_cast<Weapon*>( items[ which_item - 1 ] ) );
	} else if ( NULL != dynamic_cast<Armor*>( items[ which_item - 1 ] ) ) {
		mOS << "Equipped " << items[ which_item - 1 ]->getName() << "." << std::endl;
		mPlayer.setArmor( dynamic_cast<Armor*>( items[ which_item - 1 ] ) );
	} else {
		mOS << "That item is not equippable." << std::endl;
	}
}

void Game::playerUseItem() {
	std::vector<Item*> & items = mPlayer.getItems();

	if ( items.empty() ) {
		mOS << "You are not carrying anything to use." << std::endl;
		return;
	}

	if ( 1 == items.size() ) {
		Consumable * item = dynamic_cast<Consumable*>( items[ 0 ] );

		if ( NULL != item ) {
			mOS << "Used " << item->getName() << "." << std::endl;
			item->use( mPlayer );
			items.clear();

			// It is safe to delete the item from mItems now,
			// but we don't *have* to since it will be deleted
			// automatically on destruction.

			for ( auto i = items.begin(); items.end() != i; ++i ) {
				
			}
		} else {
			mOS << "You are not carrying anything usable." << std::endl;
		}

		return;
	}

	mOS << "Use which item? (1-" << items.size() << ") ";
	waddstr( mWindowInteractive, mOS.str().c_str() );
	mOS.str( "" );

	nocbreak();
	echo();
	nl();
	char user_input[ 128 ];
	wgetstr( mWindowInteractive, user_input );
	int which_item = atoi( user_input );
	cbreak();
	noecho();
	nonl();

	if ( which_item <= 0 || which_item > items.size() )
		return;

	Consumable * item = dynamic_cast<Consumable*>( items[ which_item - 1 ] );
	
	if ( NULL != item ) {
		mOS << "Used " << item->getName() << "." << std::endl;
		item->use( mPlayer );
		items.erase( items.begin() + ( which_item - 1 ) );
		// It is safe to delete the item from mItems now,
		// but we don't *have* to since it will be deleted
		// automatically on destruction.
	} else {
		mOS << "That item is not usable." << std::endl;
	}
}

void Game::playerViewInventory() {
	std::vector<Item*> & items = mPlayer.getItems();

	if ( items.empty() ) {
		mOS << "You are carrying 0 items." << std::endl;
		return;
	}

	mOS << "You are carrying " << items.size() << " items." << std::endl;

	for ( int i = 0; i < items.size(); ++i ) {
		Item * item = items[ i ];

		std::string item_type = "";

		if ( NULL != dynamic_cast<Weapon*>( item ) ) {
			item_type = " [Weapon";

			if ( dynamic_cast<Weapon*>( item ) == mPlayer.getWeapon() )
				item_type += "/Equipped";

			item_type += "]";
		} else if ( NULL != dynamic_cast<Armor*>( item ) ) {
			item_type = " [Armor";

			if ( dynamic_cast<Armor*>( item ) == mPlayer.getArmor() )
				item_type += "/Equipped";

			item_type += "]";
		} else if ( NULL != dynamic_cast<Consumable*>( item ) ) {
			item_type = " [Consumable]";
		}

		mOS << "[" << ( i + 1 ) << "]: " << item->getName() << item_type << std::endl;
	}
}

void Game::spawnCreatures() {
	int limit = mMaps[ mCurrentMap ]->getRooms().size() - mMaps[ mCurrentMap ]->getCreatures().size();

	for ( int i = 0; i < limit; ++i ) {
		Creature * creature = GenericFactory::getInstance().generateCreature( 2 * mMaps[ mCurrentMap ]->getLevel() );
		mMaps[ mCurrentMap ]->spawnCreature( creature );
		mCreatures.push_back( creature );
	}
}

void Game::spawnItems() {
	int limit = mMaps[ mCurrentMap ]->getRooms().size();

	for ( int i = 0; i < limit; ++i ) {
		Item * item = GenericFactory::getInstance().generateItem( 2 * mMaps[ mCurrentMap ]->getLevel() );
		mMaps[ mCurrentMap ]->spawnItem( item );
		mItems.push_back( item );
	}
}

int Game::playerVisibilityMap( int x, int y ) {
	int radius = 8;

	int px = mPlayer.getX();
	int py = mPlayer.getY();
	
	if ( ( x - px ) * ( x - px ) + ( y - py ) * ( y - py ) > radius * radius )
		return COLOR_PAIR( CP_DARK );

	return 0;
}
