/*
Author: Hak8or
Class: CSCI 235

For Myself:
No idea how to test if memory management is done correctly using chase.
*/

#include <iostream>
#include "Square_Matrix.h"
#include "Sodoku.h"

// For the testing suite.
// I am supplying my own main which contains the homework implimentation.
#include "catch.hpp"

using namespace std;

// To dump the matrix contents into terminal for easy debugging.
// This is here since I assume prof does not want non specificed methods 
// in assignment and I don't know where else to put this.
void display_matrix_contents(Square_Matrix m)
{
	cout << "Contents of matrix: \n";

	// Go through each row.
	for (int row = 0; row < m.Get_Size(); ++row)
	{
		// Formatting to chart is tilted over to the right a bit.
		cout << "   ";

		// Go through each item in the row.
		for (int item = 0; item < m.Get_Size(); ++item)
			cout << m.Get_Elem(row, item) << " ";;

		// To start displaying the next row.
		cout << endl;
	}
}

SCENARIO("Check to see if I got shifted into an alternate reality")
{
	GIVEN("An empty matrix"){
		Square_Matrix my_matrix;

		WHEN("Running sanity_check")
			THEN("It should give back the expected value")
				REQUIRE(my_matrix.sanity_check() == 2014);
	}	
}

SCENARIO("Changing components of a matrix"){
	GIVEN("a 2x2 matrix"){
		Square_Matrix my_matrix;
		my_matrix.Set_Size(2);

		WHEN("checking it's size"){
			THEN("it should have size of 2")
				REQUIRE(my_matrix.Get_Size() == 2);

			THEN("it should have no data set")
				REQUIRE(my_matrix.Get_Elem(0,0) == 0);
		}

		WHEN("Changing data"){
		my_matrix.Set_Elem(100, 0, 0);

			THEN("data should be changed")
				REQUIRE(my_matrix.Get_Elem(0,0) == 100);
		}

		WHEN("filling it with a number"){
			my_matrix.fill(5);

			THEN("it should be filled with that number"){
				bool is_filled = true;

				// It seems catch can't handle testing functions, so
				// this loop is here to check each element
				for (int i = 0; i < my_matrix.Get_Size(); ++i){
					if (!is_filled)
						break;

					for (int j = 0; j < my_matrix.Get_Size(); ++j){
						if (!is_filled)
							break;

						if (my_matrix.Get_Elem(i, j) != 5)
							is_filled = false;
					}
				}

				REQUIRE(is_filled);
			}
		}

		WHEN("resizing to 1x1"){
		my_matrix.Set_Size(1);

			THEN("size should be changed")
				REQUIRE(my_matrix.Get_Size() == 1);

			THEN("data contents should be lost")
				REQUIRE(my_matrix.Get_Elem(0,0) == 0);

			THEN("data changes take effect")
			{
				my_matrix.Set_Elem(101, 0, 0);
				REQUIRE(my_matrix.Get_Elem(0,0) == 101);
			}
		}
	}
}


SCENARIO("Given a negative size matrix, things should not explode"){
	GIVEN("An initial negative set matrix") {
		// Make the matrix
		Square_Matrix m;

		// Set the matrix to be -10 units big.
		m.Set_Size(-10);

		WHEN("the size is reset") {
			m.Set_Size(12);

			THEN("things should not blow up") {
				REQUIRE(m.sanity_check() == 2014);
			}
		}
	}
}

SCENARIO("Two unset matrix setup") {
	GIVEN("Two unset matrices") {
		Square_Matrix m1;
		Square_Matrix m2;

		WHEN("Compared") {
			THEN("are equal") {
				REQUIRE(m1 == m2);
			}
		}

		WHEN("Compared to itself") {
			THEN("is equal") {
				REQUIRE(m1 == m1);
				REQUIRE(m2 == m2);
			}
		}

		WHEN("Set to equal sizes with no data") {
			m1.Set_Size(3);
			m2.Set_Size(3);

			THEN("are equal") {
				REQUIRE(m1 == m2);
			}
		}

		WHEN("Set to equal sizes with same data") {
			m1.Set_Size(3);
			m1.Set_Elem(5, 0, 1);
			m2.Set_Size(3);
			m2.Set_Elem(5, 0, 1);

			THEN("are equal") {
				REQUIRE(m1 == m2);
			}
		}

		WHEN("Set to equal sizes with diff data") {
			m1.Set_Size(3);
			m1.Set_Elem(5, 0, 1);
			m2.Set_Size(3);
			m2.Set_Elem(6, 0, 1);

			THEN("are not equal") {
				bool foo = (m1 == m2);
				REQUIRE(!foo);
			}
		}

		WHEN("Set to equal sizes with no data") {
			m1.Set_Size(3);
			m2.Set_Size(3);

			THEN("are equal") {
				REQUIRE(m1 == m2);
			}
		}
	}
}

SCENARIO("Matrix assignment") {
	GIVEN("three equal sized matrices with diff data") {
		Square_Matrix m1;
		Square_Matrix m2;
		Square_Matrix m3;

		m1.Set_Size(3);
		m1.Set_Elem(151, 0, 2);

		m2.Set_Size(3);
		m2.Set_Elem(152, 0, 2);

		m3.Set_Size(3);
		m3.Set_Elem(153, 0, 2);

		WHEN("Comparing the matrices together") {
			bool foo1 = (m1 == m2);
			bool foo2 = (m1 == m3);
			bool foo3 = (m2 == m3);

			THEN("gives incorrect matrix result") {
				REQUIRE(!foo1);
				REQUIRE(!foo2);
				REQUIRE(!foo3);
			}
		}

		WHEN("assigned to another matrix") {
			m2 = m1;
			bool foo = (m2 == m1);

			THEN("other matrix is correct matrix result") {
				REQUIRE(foo);
			}
		}

		WHEN("assigned to itself") {
			m1 = m1;
			bool foo = (m1 == m1);

			THEN("other matrix is correct matrix result") {
				REQUIRE(foo);
			}
		}

		WHEN("doing an assignment chain") {
			m3 = m2 = m1;
			bool foo = (m3 == m1);

			THEN("other matrix is correct matrix result") {
				REQUIRE(foo);
			}
		}
	}
}

SCENARIO("Matrix addition") {
	GIVEN("Three equal sized matrices with diff data") {
		Square_Matrix m1;
		Square_Matrix m2;
		Square_Matrix correct_matrix;

		m1.Set_Size(3);
		m1.Set_Elem(151, 0, 2);

		m2.Set_Size(3);
		m2.Set_Elem(152, 0, 2);

		correct_matrix.Set_Size(3);
		correct_matrix.Set_Elem(303, 0, 2);

		WHEN("comparing the two matrices to correct_matrix") {
			bool foo1 = (m1 == m2);
			bool foo2 = (m1 == correct_matrix);
			bool foo3 = (m2== correct_matrix);

			THEN("gives incorrect matrix result") {
				REQUIRE(!foo1);
				REQUIRE(!foo2);
				REQUIRE(!foo3);
			}
		}

		WHEN("assigned to another matrix") {
			m2 = m1;
			bool foo = (m2 == m1);

			THEN("other matrix is correct matrix result") {
				REQUIRE(foo);
			}
		}

		WHEN("are added together") {
			Square_Matrix result = (m1 + m2);

			THEN("give correct solution") {
				REQUIRE(correct_matrix == result);
			}
		}


		WHEN("is added to itself") {
			Square_Matrix result = m1 + m1;
			
			correct_matrix.Set_Elem(302, 0, 2);

			THEN("give correct solution") {
				REQUIRE(correct_matrix == result);
			}
		}

		WHEN("doing an addition chain") {
			Square_Matrix temp;
			temp.Set_Size(3);
			temp.Set_Elem(100, 0, 2);

			Square_Matrix result = (m1 + m2 + temp);

			correct_matrix.Set_Elem(403, 0, 2);

			THEN("give correct solution") {
				REQUIRE(correct_matrix == result);
			}
		}
	}
}

/*
=============================================================================
						Sodoku testing
=============================================================================
*/
SCENARIO("Making a Sodoku puzzle") {
	GIVEN("A clean slate") {
		WHEN("Making a Sodoku puzzle object") {
			Sodoku puzzle;

			THEN("The Sodoku puzzle should be 2 by 2 units big") {
				REQUIRE(puzzle.get_size() == 2);
			}
		}
	}
}

SCENARIO("Modfying and accessing cells of the Sodoku puzzle") {
	GIVEN("A empty non partially filled sodoku puzzle") {
		Sodoku puzzle;

		WHEN("acessing an cell") {
			int cell_contents = puzzle.get_cell(0,1);

			THEN("cell contents should be unset(-1)") {
				REQUIRE(cell_contents == -1);
			}
		}

		WHEN("writing to an in bounds cell") {
			bool successful_write = puzzle.set_cell(0, 1, 5);

			THEN("The cell contents should be changed") {
				REQUIRE(successful_write);
				REQUIRE(puzzle.get_cell(0,1) == 5);
			}
		}
	}
}

SCENARIO("Checking an incorrect sodoku") {
	GIVEN("A sodoku puzzle with a bad row in it") {
		Sodoku puzzle;
		puzzle.set_cell(0, 0, 5);
		puzzle.set_cell(1, 0, 5);

		WHEN("checking the entire sodoku puzzle") {
			bool validity = puzzle.check_sodoku_validity();

			THEN("the return value should be false") {
				REQUIRE(!validity);
			}
		}
	}

	GIVEN("A sodoku puzzle without a bad row in it") {
		Sodoku puzzle;
		puzzle.set_cell(0, 0, 5);
		puzzle.set_cell(1, 1, 5);

		WHEN("checking the entire sodoku puzzle") {
			bool validity = puzzle.check_sodoku_validity();

			THEN("the return value should be true") {
				REQUIRE(validity);
			}
		}
	}

	GIVEN("A sodoku puzzle with a bad column in it") {
		Sodoku puzzle;
		puzzle.set_cell(0, 0, 5);
		puzzle.set_cell(0, 1, 5);

		WHEN("checking the entire sodoku puzzle") {
			bool validity = puzzle.check_sodoku_validity();

			THEN("the return value should be false") {
				REQUIRE(!validity);
			}
		}
	}

	GIVEN("A sodoku puzzle without a bad column in it") {
		Sodoku puzzle;
		puzzle.set_cell(0, 0, 5);
		puzzle.set_cell(1, 1, 5);

		WHEN("checking the entire sodoku puzzle") {
			bool validity = puzzle.check_sodoku_validity();

			THEN("the return value should be true") {
				REQUIRE(validity);
			}
		}
	}
}

SCENARIO("Fill in a few partial cells correctly.") {
	GIVEN("An empty partially filled sodoku puzzle") {
		Sodoku puzzle;

		WHEN("filling in a few cells to get started") {
			puzzle.solve_puzzle_partially(0.15);

			THEN("a few cells should be correctly filled") {
				REQUIRE(puzzle.check_sodoku_validity());
			}
		}
	}
}

SCENARIO("Checking completness of the puzzle") {
	GIVEN("An empty sodoku puzzle") {
		Sodoku puzzle;

		WHEN("checking if it is complete") {
			bool completness = puzzle.is_complete();

			THEN("it should be not complete") {
				REQUIRE(!completness);
			}
		}
	}

	GIVEN("A partially manually filled sodoku puzzle") {
		Sodoku puzzle;

		// 1 0
		// 0 .
		puzzle.set_cell(0, 0, 1);
		puzzle.set_cell(0, 1, 0);
		puzzle.set_cell(1, 0, 0);

		WHEN("checking if it is complete") {
			bool completness = puzzle.is_complete();

			THEN("it should be not complete") {
				REQUIRE(!completness);
			}
		}
	}

	GIVEN("An incorrectly filled sodoku puzzle") {
		Sodoku puzzle;

		// 1 0
		// 0 0
		puzzle.set_cell(0, 0, 1);
		puzzle.set_cell(0, 1, 0);
		puzzle.set_cell(1, 0, 0);
		puzzle.set_cell(1, 1, 0);

		WHEN("checking if it is complete") {
			bool completness = puzzle.is_complete();

			THEN("it should be not complete") {
				REQUIRE(!completness);
			}
		}
	}

	GIVEN("An correctly filled sodoku puzzle") {
		Sodoku puzzle;

		// 1 0
		// 0 1
		puzzle.set_cell(0, 0, 1);
		puzzle.set_cell(0, 1, 0);
		puzzle.set_cell(1, 0, 0);
		puzzle.set_cell(1, 1, 1);

		WHEN("checking if it is complete") {
			bool completness = puzzle.is_complete();

			THEN("it should be complete") {
				REQUIRE(completness);
			}
		}
	}
}

SCENARIO("Auto solve a sodoku with manually set consts without backtracing") {
	GIVEN("a sodoku with manually set consts") {
		Sodoku puzzle(3);

		// 1 2 .
		// . 1 .
		// . . .
		puzzle.set_const_cell(0, 0, 1);
		puzzle.set_const_cell(1, 0, 2);
		puzzle.set_const_cell(1, 1, 1);

		WHEN("trying to auto solve it.") {
			bool solved = puzzle.solve_puzzle();

			THEN("it should be solved.") {
				REQUIRE(solved);
				REQUIRE(puzzle.is_complete());
			}
		}
	}
}

SCENARIO("Auto solve a sodoku with manually set consts with backtracing") {
	GIVEN("a sodoku with manually set consts") {
		Sodoku puzzle(3);

		// . . 1
		// 2 . .
		// . . .
		puzzle.set_const_cell(2, 0, 1); // top right
		puzzle.set_const_cell(0, 1, 2); // 2 at 0, 1

		WHEN("trying to auto solve it.") {
			bool solved = puzzle.solve_puzzle();

			THEN("it should be solved.") {
				REQUIRE(solved);
				REQUIRE(puzzle.is_complete());
			}
		}
	}
}

SCENARIO("Auto solve a sodoku with manually set consts together and requiring backtracking") {
	GIVEN("a sodoku with manually set consts together and requiring backtracking") {
		Sodoku puzzle(3);

		// 0 2 .
		// 2 . .
		// . . .
		puzzle.set_const_cell(0, 0, 0); // top left
		puzzle.set_const_cell(1, 0, 2); // top right
		puzzle.set_const_cell(0, 1, 2); // 2 at 0, 1

		WHEN("trying to auto solve it.") {
			bool solved = puzzle.solve_puzzle();

			THEN("it should be solved.") {
				REQUIRE(solved);
				REQUIRE(puzzle.is_complete());
			}
		}
	}

	GIVEN("a larger sodoku with manually set consts together and requiring backtracking") {
		Sodoku puzzle(4);

		//     0 1 2 3  
		// 0 | . 3 2 . | 0
		// 1 | . . . . | 1
		// 2 | 1 0 3 2 | 2
		// 3 | . . . . | 3
		//     0 1 2 3  

		// Row 0 
		puzzle.set_const_cell(1, 0, 3);
		puzzle.set_const_cell(2, 0, 2);

		// Row 2
		puzzle.set_const_cell(0, 2, 1);
		puzzle.set_const_cell(1, 2, 0);
		puzzle.set_const_cell(2, 2, 3);
		puzzle.set_const_cell(3, 2, 2);

		WHEN("trying to auto solve it.") {
			bool solved = puzzle.solve_puzzle();

			THEN("it should be solved.") {
				REQUIRE(solved);
				REQUIRE(puzzle.is_complete());
			}
		}
	}
}

SCENARIO("Auto solve a larger sodoku with manually set consts that requires a string of backtraces") {
	GIVEN("a sodoku with manually set consts") {
		Sodoku puzzle(9);

		// 0 . 1 . . . . . . // 0
		// . 2 . . . . . . . // 1
		// . . . . . . . . . // 2
		// . . . . . . . . . // 3
		// . . . . . . . . . // 4
		// . . . . . . . . . // 5
		// . . . . . . . . . // 6
		// . . . . . . . 6 . // 7
		// . . . . . . . . . // 8
		puzzle.set_const_cell(0, 0, 0);
		puzzle.set_const_cell(2, 0, 1);
		puzzle.set_const_cell(1, 1, 2);
		puzzle.set_const_cell(6, 7, 7);

		WHEN("trying to auto solve it.") {
			bool solved = puzzle.solve_puzzle();

			THEN("it should be solved.") {
				REQUIRE(solved);
				REQUIRE(puzzle.is_complete());
			}
		}
	}

	GIVEN("a sodoku with manually set consts") {
		Sodoku puzzle(9);

		//   0 1 2 3 4 5 6 7 8  
		// | . . 1 . . . . . . | 0
		// | 2 . . . . . . . . | 1
		// | . . . . . . . . . | 2 
		// | . . . . . . . . . | 3
		// | . . . . . . . . . | 4 
		// | . . . 0 . . . . . | 5
		// | . . . . . . . . . | 6
		// | . . . . . . . 5 . | 7
		// | . . . . . . . . . | 8
		//   0 1 2 3 4 5 6 7 8  

		//     0 1 2 3 4 5 6 7 8  
		// 0 | 0 2 1 3 4 5 6 7 8  | 0
		// 1 | 2 0 3 1 5 4 7 8 6  | 1
		// 2 | 1 3 0 2 6 7 8 4 5  | 2
		// 3 | 3 1 2 4 0 8 5 6 7  | 3
		// 4 | 4 5 6 7 8 0 1 2 3  | 4
		// 5 | 5 4 8 0 7 6 2 3 1  | 5
		// 6 | 6 7 5 8 1 2 3 0 4  | 6
		// 7 | 7 8 4 6 3 1 0 5 2  | 7
		// 8 | 8 6 7 5 2 3 4 1 0  | 8
		//     0 1 2 3 4 5 6 7 8  

		puzzle.set_const_cell(2, 0, 1);
		puzzle.set_const_cell(0, 1, 2);
		puzzle.set_const_cell(3, 5, 0);
		puzzle.set_const_cell(7, 7, 5);

		WHEN("trying to auto solve it.") {
			bool solved = puzzle.solve_puzzle();

			THEN("it should be solved.") {
				REQUIRE(solved);
				REQUIRE(puzzle.is_complete());
			}
		}
	}
}

SCENARIO("Trying to solve an unsolvable sodoku will fail"){
	GIVEN("An unsolvable sodoku") {
		Sodoku puzzle(4);

		//     0 1 2 3  
		// 0 | . 3 2 . | 0
		// 1 | . . 0 2 | 1
		// 2 | 1 0 3 . | 2
		// 3 | . . . . | 3
		//     0 1 2 3  

		// Row 0 
		puzzle.set_const_cell(1, 0, 3);
		puzzle.set_const_cell(2, 0, 2);

		// Row 1 
		puzzle.set_const_cell(2, 1, 0);
		puzzle.set_const_cell(3, 1, 2);

		// Row 2
		puzzle.set_const_cell(0, 2, 1);
		puzzle.set_const_cell(1, 2, 0);
		puzzle.set_const_cell(2, 2, 3);

		WHEN("trying to auto solve it") {
			bool solved = puzzle.solve_puzzle();

			THEN("it should not be able to be solved.") {
				REQUIRE(!solved);
				REQUIRE(!puzzle.is_complete());
			}
		}
	}
}

SCENARIO("Solve a few small matrices from scratch.") {
	GIVEN("A sodoku of size 1") {
		Sodoku puzzle(1);

		WHEN("trying to auto solve it.") {
			bool solved = puzzle.solve_puzzle();

			THEN("it should be solved.") {
				REQUIRE(solved);
				REQUIRE(puzzle.is_complete());
			}
		}

		WHEN("trying to auto solve it with prefil"){
			puzzle.solve_puzzle_partially(0.25);
			bool solved = puzzle.solve_puzzle();

			THEN("it should be solved.") {
				REQUIRE(solved);
				REQUIRE(puzzle.is_complete());
			}
		}
	}

	GIVEN("A sodoku of size 2") {
		Sodoku puzzle(2);

		WHEN("trying to auto solve it.") {
			bool solved = puzzle.solve_puzzle();

			THEN("it should be solved.") {
				REQUIRE(solved);
				REQUIRE(puzzle.is_complete());
			}
		}

		WHEN("trying to auto solve it with prefil"){
			puzzle.solve_puzzle_partially(0.25);
			bool solved = puzzle.solve_puzzle();

			THEN("it should be solved.") {
				REQUIRE(solved);
				REQUIRE(puzzle.is_complete());
			}
		}
	}

	GIVEN("A sodoku of size 3") {
		Sodoku puzzle(3);

		WHEN("trying to auto solve it.") {
			bool solved = puzzle.solve_puzzle();

			THEN("it should be solved.") {
				REQUIRE(solved);
				REQUIRE(puzzle.is_complete());
			}
		}

		WHEN("trying to auto solve it with prefil"){
			puzzle.solve_puzzle_partially(0.25);
			bool solved = puzzle.solve_puzzle();

			THEN("it should be solved.") {
				REQUIRE(solved);
				REQUIRE(puzzle.is_complete());
			}
		}
	}
}

// This is an example of a sodoku puzzle which is very hard to
// solve, taking a very long time if running on a single core
// but taking under a minute assuming using all 4 cores of an
// I5-3570k running at stock speeds.
//
// Kept here if I will ever try to speed this up using any of the better
// methods.
//
// Manually disabled. Uncomment to enable.
SCENARIO("A manually partially filled puzzle which is too hard to solve for now.") {
	GIVEN("a sodoku with manually set consts that usually took forever") {
		Sodoku puzzle(9);

		//     0 1 2 3 4 5 6 7 8  
		// 0 | . . . 0 5 . . . .  | 0
		// 1 | . 2 . . . . 5 . .  | 1
		// 2 | . . . . . . . . .  | 2
		// 3 | . . 4 . . . . . .  | 3
		// 4 | . . 8 . . . . . .  | 4
		// 5 | . . . . 0 . 2 . .  | 5
		// 6 | . . 7 . . . . . 5  | 6
		// 7 | . . . . . . 0 . .  | 7
		// 8 | . . 6 . . . . . .  | 8
		//     0 1 2 3 4 5 6 7 8  


		// Row 0   | . . . 0 5 . . . .  |
		puzzle.set_const_cell(3, 0, 0);
		puzzle.set_const_cell(4, 0, 5);

		// Row 1   | . 2 . . . . 5 . .  |
		puzzle.set_const_cell(1, 1, 2);
		puzzle.set_const_cell(6, 1, 5);

		// Row 3   | . . 4 . . . . . .  |
		puzzle.set_const_cell(2, 3, 4);

		// Row 4   | . . 8 . . . . . .  |
		puzzle.set_const_cell(2, 4, 8);

		// Row 5   | . . . . 0 . 2 . .  |   
		puzzle.set_const_cell(4, 5, 0);
		puzzle.set_const_cell(6, 5, 2);

		// Row 6   | . . 7 . . . . . 5  |   
		puzzle.set_const_cell(2, 6, 7);
		puzzle.set_const_cell(8, 6, 5);

		// Row 7  | . . . . . . 0 . .  |   
		puzzle.set_const_cell(6, 7, 0);

		// Row 8  | . . 6 . . . . . .  |  
		puzzle.set_const_cell(2, 8, 6);

		WHEN("trying to auto solve it.") {
			// bool solved = puzzle.solve_puzzle();

			THEN("it should be solved.") {
				REQUIRE(true);
				// REQUIRE(solved);
				// REQUIRE(puzzle.is_complete());
			}
		}
	}
}
