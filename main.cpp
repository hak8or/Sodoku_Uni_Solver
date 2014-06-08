/*
Author: Marcin Ziemianowicz
Class: CSCI 235 (Descrete Analysis 2) Section 01
Professor: Pavel Shostak

Compilation Instructions: 
To compile: make
To clean: make clean
To run: ./Sodoku
To do it all: make clean && make && ./Sodoku


|-Square_Matrix
----------------
| This project relies on the usage of our previous library, Square_Matrix, 
| as what holds the contents of the sodoku puzzle.
----------------


|- Partial solve algorithms
----------------------------
| This uses two "filling" algorithms. The first is used to fill just a small
| section of the puzzle to get us started. It should be used for only small
| percentages of the puzzle because it fills the puzzle using a random brute
| force method which is very inefficiant.
|
| This does NOT guarantee a solvable puzzle. To check if it would be
| solvable would require to try to actually solve it, which would
| defeat the purpose of partially filling it. If the user wants partial
| fill to guarantee a solvable puzzle, then it is up to the user to do it
| him/her self by partially filling a puzzle, try to solve it, and if fails
| then try to partially fill it again.
----------------------------


|- The actual filling algorithm (backtrack)
--------------------------------------------
| While the assignment requires either recursion or backtrack to be used, my
| method uses both. The general idea is it looks clockwise for a node which
| was not set using partial_fill and attempts to fill it with a number that
| follows the rules of no other same number being in the same column and row.
| If it cannot find a number which satisfies those conditions for that node, 
| then it goes back in a clockwise direction to a node not set using
| partial solve and tries to find another number for the now current node.
| 
| It can go like this for the entire sodoku puzzle, once it finishes the
| last element, then it is considered that the puzzle has been fully solved.
| If it went all the way back to the earliest cell not filled with partial_fill
| and incrememnted to the highest possible value, then it is considered that 
| the puzzle in its current form is not solvable.
--------------------------------------------


|- The actual filling algorithm (recursive)
--------------------------------------------
| There is also a dash of recursion solely for finding the next or previous node.
| It goes forwards and back a cell as expected, in a clockwise and counter 
| clockwise direction, but if it finds that the next or previous node was filled
| using partial_fill then it calls itself to go even further forwards or backwards
| a cell. This behavior allows any number of partial_fill cells being "in the way"
| without any issues.
--------------------------------------------

|- Very rough and quick algo "complexity" estimation.
------------------------------------------------------
| Using my algo, the worst case total cell content increment count would be
| size^(size^2), or if taking into account pre filled cells size^(size^2 - prefilled).
| This is one amazingly inefficiant algorythm!
------------------------------------------------------


|- Tests
---------
| There are a series of extensive tests both for Square_Matrix and Sodoku. I was
| not sure if I should have included the tests for Square_Matrix but decided to
| add them anyways. These tests are run automatically in the beginning of the 
| program when executed.
|
| ------- Test Status: PASS
| Tested on: Ubuntu raring64 (13.04) 64bit, gcc 4.8.1, 512MB ram.
---------


|- Misc
--------
| This is on: https://bitbucket.org/hak8or/soduku/overview
| While the repo is public, no one but myself worked on it for the entire duration.
| It is open solely for if blackboard looses my assignment and if requested I will
| make the repository private or even delete the repository.
|
| I also threw in my notes onto the VCS where this is stored.
|
| There were troubles with using version control for this since I originally was 
| just putting everything into a develop branch but later wanted to use gitflow 
| which did not work out too well so it remained as is. The branching and merging
| is not miraculous.
|
| The main filling algo was rewritten a few times due to me wanting to make it more
| clear, faster, or cleaner. While I did update the comments to reflect the changes,
| some old comments from previous implimentations might remain.
|
| The -std=c++0x flag was added due to a bug for converting int to string from the
| standard library. 
| http://stackoverflow.com/questions/12975341/to-string-is-not-a-member-of-std-says-so-g
--------

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
				"something because this might take a long time.";

	// Makes a sodoku puzzle of the user specifed size.
	Sodoku puzzle(size);

	// Display the empty puzzle.
	puzzle.display("\nThis is your empty sodoku puzzle.");

	// Fills some randomly selected yet valid starting cells.
	cout << "Lets fill 25% of it with some random yet valid starting cells.\n";

	// Tell the user about the future loop.
	cout << "\n\t NOTE:\n";
	cout << "Partial fill does not guarantee to supply a valid puzzle. This loop "
	cout << "will try to\n find a solvable partially filled puzzle by trying to "
	cout << "actually solve it and if it\n can't solve it, then it partially "
	cout << "fills it again.\n"
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
	partially_filled_puzzle.display("Partially pre-filled puzzle.");

	// And now show the filled puzle!
	puzzle.display("Solved puzzle");

	// Show a heatmap which displays how many times we tried a value in each cell.
	puzzle.display_heatmap("# of times a value was tried per cell.");

	// And lastly show how many totall attempts there were to write to a cell.
	// This gives a good idea for how "hard" it was to solve the puzzle.
	cout << "Whew! That took " + std::to_string(puzzle.get_amount_of_steps()) 
			+ " steps!\n";

	return 0;
}
