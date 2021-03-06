/*
 *	Author: Hak8or
 *	Class: CSCI 235 Descrete Analysis 2
 *
 *	Compilation Instructions: 
 *		To compile: make
 *		To clean: make clean
 *		To run: ./Sodoku
 *		To test: make test
 *		To generate docs: make docs
 *		To do it all: make clean && make && make docs && make test
 */

#include <iostream>
#include "Square_Matrix.h"
#include "Sodoku.h"

// This handles the testing suite. 
#define CATCH_CONFIG_RUNNER // Supplying my own main.
#include "catch.hpp"

using namespace std;

// Homework implimentation. No command line arguments taken, added here for 
// compliance though.
int main(int argc, char *argv[]) {
	cout << "Running self tests ... \n";

	// If we did not get a 0 then the tests failed.
	if (Catch::Session().run( argc, argv ) != 0)
		cout << "Self tests failed, the program will still run but the results"
				 "are probably incorrect." << endl;

	// Get the users requested size of the sodoku puzzle.
	int size;
	cout << "Size of Sodoku puzzle: ";
	cin >> size;

	// If the size was less than 1, then say that the size is bad and request
	// a new size.
	while(size < 1) {
		cout << "Size selection was not valid, please type in a resonable size: ";
		cin >> size;
	}

	// If the given size is expected to generate a puzzle which takes a long time
	// to solve, warn user.
	if(size >= 10)
		cout << "You chose a somewhat large matrix. Go get a cup of coffee or "
				"something because this might take a long time." << endl;

	// Makes a sodoku puzzle of the user specifed size.
	Sodoku puzzle(size);

	// Display the empty puzzle.
	cout << "This is your empty sodoku puzzle." << endl;
	puzzle.display();

	// Fills some randomly selected yet valid starting cells.
	cout << "Lets fill 25% of it with some random yet valid starting cells.\n";

	// Tell the user about the future loop.
	cout << "\n\t NOTE:\n"
		"Partial fill does not guarantee to supply a valid puzzle. This loop "
		"will try to find a solvable partially filled puzzle by trying to "
		"actually solve it and if it can't solve it, then it partially "
		"fills it again.\n \n";
	puzzle.solve_puzzle_partially(0.25);

	// This will hold what the puzzle looked like before it was solved.
	Sodoku partially_filled_puzzle(size);
	partially_filled_puzzle = puzzle;

	// Since partial solve does not guarantee to give a solvable puzzle, do a 
	// loop till it finds a solvable partially solved puzzle
	while(!puzzle.solve_puzzle())
	{
		cout << "Found an unsolvable puzzle, making a new one...\n";
		puzzle.wipe();
		puzzle.solve_puzzle_partially(0.25);
		partially_filled_puzzle = puzzle;
	}

	// Show the partially filled puzzle first.
	cout << "Partially pre-filled puzzle." << endl;
	partially_filled_puzzle.display();

	// And now show the filled puzle!
	cout << "Solved puzzle" << endl;
	puzzle.display();

	// Show a heatmap which displays how many times we tried a value in each cell.
	cout << "# of times a value was tried per cell." << endl;
	puzzle.display_heatmap();

	// And lastly show how many totall attempts there were to write to a cell.
	// This gives a good idea for how "hard" it was to solve the puzzle.
	cout << "Whew! That took " + std::to_string(puzzle.get_amount_of_steps()) 
			+ " steps!\n";

	return 0;
}
