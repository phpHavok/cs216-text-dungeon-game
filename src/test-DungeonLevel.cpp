#include <iostream>
#include <random>
#include <vector>
#include "inc/Tile.h"
#include "inc/DungeonLevel.h"
using namespace std;

void markTile( DungeonLevel & level, int row, int column ) {
	vector<vector<Tile>> & tiles = level.getTiles();
	tiles[ row ][ column ].setSymbol( 'X' );

	// Up
	if ( row - 1 >= 0 ) {
		char symbol_up = tiles[ row - 1 ][ column ].getSymbol();

		if ( '.' == symbol_up || '#' == symbol_up )
			markTile( level, row - 1, column );
	}

	// Down
	if ( row + 1 < tiles.size() ) {
		char symbol_down = tiles[ row + 1 ][ column ].getSymbol();

		if ( '.' == symbol_down || '#' == symbol_down )
			markTile( level, row + 1, column );

	}

	// Left
	if ( column - 1 >= 0 ) {
		char symbol_left = tiles[ row ][ column - 1 ].getSymbol();

		if ( '.' == symbol_left || '#' == symbol_left )
			markTile( level, row, column - 1 );

	}

	// Right
	if ( column + 1 < tiles[ row ].size() ) {
		char symbol_right = tiles[ row ][ column + 1 ].getSymbol();

		if ( '.' == symbol_right || '#' == symbol_right )
			markTile( level, row, column + 1 );

	}
}

bool levelMustBe79By20( DungeonLevel level ) {
	const vector<vector<Tile>> & tiles = level.getTiles();

	if ( 20 != tiles.size() )
		return false;

	for ( unsigned int r = 0; r < tiles.size(); ++r ) {
		if ( 79 != tiles[ r ].size() )
			return false;
	}

	return true;
}

bool levelMustHaveAtLeast200RoomTiles( DungeonLevel level ) {
	const vector<vector<Tile>> & tiles = level.getTiles();

	int num_room_tiles = 0;

	for ( unsigned int r = 0; r < tiles.size(); ++r ) {
		for ( unsigned int c = 0; c < tiles[ r ].size(); ++c ) {
			char symbol = tiles[ r ][ c ].getSymbol();

			// < and > can cover up the room tiles.
			if ( '.' == symbol || '<' == symbol || '>' == symbol )
				num_room_tiles++;
		}
	}

	return num_room_tiles >= 200;
}

// TODO: implement.
bool allRoomsMustBeRectangularAndNotOverlap( DungeonLevel level ) {
	return true;
}

bool allRoomsMustBeConnected( DungeonLevel level ) {
	const vector<vector<Tile>> & tiles = level.getTiles();

	bool passableTileFound = false;

	// Find a passable tile.
	for ( int r = 0; r < tiles.size(); ++r ) {
		for ( int c = 0; c < tiles[ r ].size(); ++c ) {
			char symbol = tiles[ r ][ c ].getSymbol();

			if ( '.' == symbol || '#' == symbol ) {
				// Begin recursive marking of passable tiles.
				markTile( level, r, c );
				passableTileFound = true;
				break;
			}
		}

		if ( passableTileFound )
			break;
	}

	// Find a passable tile again. Should fail this time if connected.
	for ( int r = 0; r < tiles.size(); ++r ) {
		for ( int c = 0; c < tiles[ r ].size(); ++c ) {
			char symbol = tiles[ r ][ c ].getSymbol();

			if ( '.' == symbol || '#' == symbol )
				return false;
		}
	}

	return true;
}

// TODO: implement.
bool allRoomsMustContainAtLeast16Tiles( DungeonLevel level ) {
	return true;
}

bool levelMustContainUpstairsAndDownstairs( DungeonLevel level ) {
	const vector<vector<Tile>> & tiles = level.getTiles();

	int upstairs_found = 0;
	int downstairs_found = 0;

	for ( int r = 0; r < tiles.size(); ++r ) {
		for ( int c = 0; c < tiles[ r ].size(); ++c ) {
			char tile = tiles[ r ][ c ].getSymbol();

			if ( '<' == tile )
				upstairs_found++;
			else if ( '>' == tile )
				downstairs_found++;
		}
	}

	return 1 == upstairs_found && 1 == downstairs_found;
}

int main( int argc, char * argv[] ) {
	int tests_total = 1;

	if ( argc >= 2 ) {
		tests_total = atoi( argv[ 1 ] );

		if ( tests_total < 1 )
			tests_total = 1;
	}

	int tests_passed = 0;
	int tests_failed = 0;

	for ( int i = 0; i < tests_total; ++i ) {
		DungeonLevel level;

		bool a_test_has_failed = false;

		if ( levelMustBe79By20( level ) ) {
			tests_passed++;
		} else {
			tests_failed++; 

			cout << "levelMustBe79By20() failed." << endl;
			a_test_has_failed = true;
		}

		if ( levelMustHaveAtLeast200RoomTiles( level ) ) {
			tests_passed++;
		} else {
			tests_failed++; 

			cout << "levelMustHaveAtLeast200RoomTiles() failed." << endl;
			a_test_has_failed = true;
		}

		if ( allRoomsMustBeRectangularAndNotOverlap( level ) ) {
			tests_passed++;
		} else {
			tests_failed++; 

			cout << "allRoomsMustBeRectangularAndNotOverlap() failed." << endl;
			a_test_has_failed = true;
		}

		if ( allRoomsMustBeConnected( level ) ) {
			tests_passed++;
		} else {
			tests_failed++; 

			cout << "allRoomsMustBeConnected() failed." << endl;
			a_test_has_failed = true;
		}

		if ( allRoomsMustContainAtLeast16Tiles( level ) ) {
			tests_passed++;
		} else {
			tests_failed++; 

			cout << "allRoomsMustContainAtLeast16Tiles() failed." << endl;
			a_test_has_failed = true;
		}

		if ( levelMustContainUpstairsAndDownstairs( level ) ) {
			tests_passed++;
		} else {
			tests_failed++; 

			cout << "levelMustContainUpstairsAndDownstairs() failed." << endl;
			a_test_has_failed = true;
		}

		if ( a_test_has_failed )
			level.draw( cout );
	}

	cout << "DungeonLevel: " << tests_passed << " tests passed. " << tests_failed << " tests failed." << endl;

	return 0;
}
