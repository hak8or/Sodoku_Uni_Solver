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
-----------------
| This project relies on the usage of our previous library, Square_Matrix, 
| as what holds the contents of the sodoku puzzle. I added get_row and get_column
| which returns a vector of those elements, and a fill function which fills the
| entire matrix with an int all in one go.
-----------------


|- Partial fill algorithms
---------------------------
| This uses two "filling" algorithms. The first is used to fill just a small
| section of the puzzle to get us started. It is limited to only a small
| percentage of the puzzle because it fills the puzzle using a random brute
| force method. This was done for simplicities sake and because it would
| fail only for higher percentages.
---------------------------


|- The actual filling algorithm (backtrack)
---------------------------------------------
| While the assignment requires either recursion or backtrack to be used, my
| method uses both. The general idea is it looks clockwise for a node which
| was not set using partial_fill and attempts to fill it with a number that
| follows the rules of no other same number being in the same column and row.
| If it cannot find a number which satisfies those conditions for that node, 
| then it goes back in a clockwise direction to a node not set using
| partial_fill and tries to find another number for the now current node.
| 
| It can go like this for the entire sodoku puzzle, once it finishes the
| last element, then it is considered that the puzzle has been fully solved.
| If it went all the way back to the earliest cell not filled with partial_fill
| then it is considered that the puzzle in its current form is not solvable.
---------------------------------------------


|- The actual filling algorithm (recursive)
--------------------------------------------
| There is also a dash of recursion solely for finding the next or previous node.
| It goes forwards and back a cell as expected, in a clockwise and counter 
| clockwise direction, but if it finds that the next or previous node was filled
| using partial_fill then it calls itself to go even further forwards or backwards
| a cell. This behavior allows any number of partial_fill cells being "in the way"
| without any issues.
--------------------------------------------


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
| I also threw in my notes onto the VCS where this is stored.
|
| This is on: https://bitbucket.org/hak8or/soduku/overview
| While the repo is public, no one but myself worked on it for the entire duration.
| It is open solely for if blackboard looses my assignment and if requested I will
| make the repository private or even delete the repository.
|
| There were troubles with using version control for this since I originally was just
| putting everything into a develop branch but later wanted to use gitflow which did
| work out too well so it remained as is. The branching and merging is not miraculous.
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
int main(int argc, char *argv[])
{
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
	while(size < 1)
	{
		cout << "Size selection was not valid, please type in a resonable size: ";
		cin >> size;
	}

	if(size == 11)
		cout << "You chose a somewhat large matrix. It could take a minute or "
				"two to solve depending on how much backtracing is needed.";

	if(size > 11)
		cout << "You chose a somewhat large matrix. Go get a cup of coffee or "
				"something because this will take a long time.";

	// Makes a sodoku puzzle of the user specifed size.
	Sodoku puzzle(size);

	// Display the empty puzzle.
	puzzle.display("\nThis is your empty sodoku puzzle.");

	// Fills some randomly selected yet valid starting cells.
	cout << "Lets fill it with some starting cells ...\n";
	puzzle.solve_puzzle_partially(0.15); // It being 15% in this case.
	puzzle.display(); // And show it back to the user.

	// Solve the puzzle and if it was solved display the contents. Otherwise
	// show that it was unsolvable.
	cout << "And now we solve it! \n";
	if (puzzle.solve_puzzle())
	{
		puzzle.display();
		cout << "Whew! That took " << puzzle.get_amount_of_steps() << " steps!\n";
	}
	else 
		cout << "The generated puzzle is unsolvable.";

	return 0;
}
