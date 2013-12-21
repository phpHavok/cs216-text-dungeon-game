#include "inc/DungeonLevel.h"
#include <cmath>
#include <algorithm>
#include "inc/Creature.h"
#include "inc/Item.h"
#include "inc/GenericFactory.h"
#include "inc/Random.h"

DungeonLevel::DungeonLevel()
	: mLevel( 1 )
{
	// Get an instance to the random number generator.
	Random & random = Random::getInstance();

	// Allocate initial raw tiles for the map.
	for ( int r = 0; r < HEIGHT; ++r ) {
		std::vector<Tile> tempTiles;
		
		for ( int c = 0; c < WIDTH; ++c )
			tempTiles.push_back( Tile() );

		mTiles.push_back( tempTiles );
	}

	// Fabricate the virtual grid that the rooms will lie on.
	int num_grid_columns = ( 0 == random.rand( 0, 1 ) ) ? 2 : 4;
	int num_grid_rows = 3;
	int num_grid_cells = num_grid_columns * num_grid_rows;

	int grid_column_gutter = num_grid_columns - 1;
	int grid_row_gutter = num_grid_rows - 1;

	int grid_cell_width = ( WIDTH - grid_column_gutter ) / num_grid_columns;
	int grid_cell_height = ( HEIGHT - grid_row_gutter ) / num_grid_rows;
	int grid_cell_area = grid_cell_width * grid_cell_height;

	// Select a number of rooms to generate between 2 and the number of cells.
	int num_rooms = random.rand( 2, num_grid_cells );
	int min_area_per_room = static_cast<int>( ceil( 200.0f / num_rooms ) );

	// Make some cells. We will reorder this to pick random cells for our rooms
	// if we have fewer rooms than cells.
	std::vector<int> cells( num_grid_cells, 0 );
	for ( unsigned int i = 0; i < cells.size(); ++i )
		cells[ i ] = i;
	
	auto shuffle_vector = [&]( int i ) { return random.rand( 0, i - 1 ); };

	random_shuffle( cells.begin(), cells.end(), shuffle_vector );

	// For each room...
	for ( int i = 0; i < num_rooms; ++i ) {
		// Pick random dimensions for the room, at least 3x3 though.
		int prospective_width = random.rand( 3, grid_cell_width );
		int prospective_height = random.rand( 3, grid_cell_height );

		// If the chosen dimensions do not create the minimum area per room,
		// logically scale the room up until it does.
		while ( prospective_width * prospective_height < min_area_per_room ) {
			int offset_width = grid_cell_width - prospective_width;
			int offset_height = grid_cell_height - prospective_height;

			if ( offset_width > offset_height )
				prospective_width++;
			else
				prospective_height++;
		}

		// Pick a random location in the cell to place the room.
		int cell = cells[ i ];
		int cell_c = cell % num_grid_columns;
		int cell_r = cell / num_grid_columns;
		int cell_x = cell_c * ( grid_cell_width + 1 );
		int cell_y = cell_r * ( grid_cell_height + 1 );

		int room_x = cell_x + random.rand( 0, grid_cell_width - prospective_width );
		int room_y = cell_y + random.rand( 0, grid_cell_height - prospective_height );

		// Iterate over the room dimensions, creating room tiles.
		for ( int r = room_y; r < room_y + prospective_height; ++r ) {
			for ( int c = room_x; c < room_x + prospective_width; ++c ) {
				// TODO: Replace this with a completely different tile.
				mTiles[ r ][ c ].setSymbol( '.' );
			}
		}

		// Save this room.
		mRooms.push_back( Rect<int>( room_x, room_y, prospective_width, prospective_height ) );
	}

	// Generate a tunnel path between one room to the next room in the list of rooms.
	// Note that this produces really interesting tunnels since the rooms are placed
	// randomly over the cells. This also ensures that there is always at least one path
	// to get from any room to any other room.
	for ( int i = 0; i < num_rooms - 1; ++i ) {
		// Operate on the current room and the next room in the list of rooms.
		Rect<int> current_rect = mRooms[ i ];
		Rect<int> next_rect = mRooms[ i + 1 ];

		// Pick a random (x,y) coordinate pair in the first room to begin the tunnel.
		int initial_x1 = current_rect.getX() + random.rand( 0, current_rect.getW() - 1 );
		int initial_y1 = current_rect.getY() + random.rand( 0, current_rect.getH() - 1 );

		// Pick a random (x,y) coordinate pair in the second room to end the tunnel.
		int initial_x2 = next_rect.getX() + random.rand( 0, next_rect.getW() - 1 );
		int initial_y2 = next_rect.getY() + random.rand( 0, next_rect.getH() - 1 );

		// Calculate the x and y offsets of the two coordinate pairs calculated previously.
		int delta_x = initial_x2 - initial_x1;
		int delta_y = initial_y2 - initial_y1;

		// From the delta's, determine (simply) if one should move in the positive or
		// negative direction to get from the first coordinate to the second.
		int dx = ( delta_x < 0 ) ? -1 : 1;
		int dy = ( delta_y < 0 ) ? -1 : 1;

		// Begin at the first coordinate pair and move to the second coordinate pair,
		// adding tunnel tiles if necessary/appropriate.
		int r;

		for ( r = initial_y1; r != initial_y2; r += dy ) {
			if ( '.' != mTiles[ r ][ initial_x1 ].getSymbol() )
				mTiles[ r ][ initial_x1 ].setSymbol( '#' );
		}

		for ( int c = initial_x1; c != initial_x2; c += dx ) {
			if ( '.' != mTiles[ r ][ c ].getSymbol() )
				mTiles[ r ][ c ].setSymbol( '#' );
		}
	}

	// Now pick a couple of random places for the stair cases. To do this, we shuffle the
	// rooms and then just use the first and second room in the newly shuffled list. Since
	// we know we will always have at least 2 rooms, we can be sure that the upstairs and
	// downstairs will never appear in the same room.
	std::vector<Rect<int>> rooms( mRooms );
	random_shuffle( rooms.begin(), rooms.end(), shuffle_vector );

	// Pick a random location in the rooms to place the upstairs and downstairs.
	// The only restriction is that they not lie on the very edge of the room, because
	// it looks better that way.
	int rand_row1 = rooms[ 0 ].getY() + random.rand( 1, rooms[ 0 ].getH() - 2 );
	int rand_col1 = rooms[ 0 ].getX() + random.rand( 1, rooms[ 0 ].getW() - 2 );
	int rand_row2 = rooms[ 1 ].getY() + random.rand( 1, rooms[ 1 ].getH() - 2 );
	int rand_col2 = rooms[ 1 ].getX() + random.rand( 1, rooms[ 1 ].getW() - 2 );

	mTiles[ rand_row1 ][ rand_col1 ].setSymbol( '>' );
	mTiles[ rand_row2 ][ rand_col2 ].setSymbol( '<' );
}

DungeonLevel::DungeonLevel( const DungeonLevel & other ) {
	mTiles = other.mTiles;
	mCreatures = other.mCreatures;
	mLevel = other.mLevel;
	mRooms = other.mRooms;
}

DungeonLevel::~DungeonLevel() {}

void DungeonLevel::draw( std::ostream & os ) const {
	std::vector<std::vector<char>> tiles = draw();

	for ( int r = 0; r < tiles.size(); ++r ) {
		for ( int c = 0; c < tiles[ r ].size(); ++c ) {
			os << tiles[ r ][ c ];
		}

		os << std::endl;
	}
}

std::vector<std::vector<char>> DungeonLevel::draw() const {
	std::vector<std::vector<char>> output;

	for ( int r = 0; r < mTiles.size(); ++r ) {
		std::vector<char> temp;

		for ( int c = 0; c < mTiles[ r ].size(); ++c ) {
			temp.push_back( mTiles[ r ][ c ].draw() );
		}

		output.push_back( temp );
	}

	return output;
}

void DungeonLevel::spawnCreature( Creature * creature ) {
	int x, y;
	Random & random = Random::getInstance();

	do {
		const Rect<int> & room = mRooms[ random.rand( 0, mRooms.size() - 1 ) ];
		x = room.getX() + random.rand( 0, room.getW() - 1 );
		y = room.getY() + random.rand( 0, room.getH() - 1 );
	} while ( NULL != mTiles[ y ][ x ].getCharacter() );

	creature->setX( x );
	creature->setY( y );
	mTiles[ y ][ x ].setCharacter( creature );
	mCreatures.push_back( creature );
}

void DungeonLevel::spawnItem( Item * item ) {
	Random & random = Random::getInstance();

	const Rect<int> & room = mRooms[ random.rand( 0, mRooms.size() - 1 ) ];
	int x = room.getX() + random.rand( 0, room.getW() - 1 );
	int y = room.getY() + random.rand( 0, room.getH() - 1 );

	std::vector<Item*> & items = mTiles[ y ][ x ].getItems();
	items.push_back( item );
}

void DungeonLevel::spawnItem( Item * item, int x, int y ) {
	std::vector<Item*> & items = mTiles[ y ][ x ].getItems();
	items.push_back( item );
}

void DungeonLevel::setTiles( const std::vector<std::vector<Tile>> & tiles ) {
	mTiles = tiles;
}

const std::vector<std::vector<Tile>> & DungeonLevel::getTiles() const {
	return mTiles;
}

std::vector<std::vector<Tile>> & DungeonLevel::getTiles() {
	return mTiles;
}

void DungeonLevel::setLevel( int level ) {
	mLevel = level;
}

int DungeonLevel::getLevel() const {
	return mLevel;
}

const std::vector<Rect<int>> & DungeonLevel::getRooms() const {
	return mRooms;
}

void DungeonLevel::setCreatures( const std::vector<Creature*> & creatures ) {
	mCreatures = creatures;
}

const std::vector<Creature*> & DungeonLevel::getCreatures() const {
	return mCreatures;
}

std::vector<Creature*> & DungeonLevel::getCreatures() {
	return mCreatures;
}
