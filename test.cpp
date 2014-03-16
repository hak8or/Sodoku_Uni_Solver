/*
Author: Marcin Ziemianowicz
Class: CSCI 235 (Descrete Analysis 2) Section 01
Professor: Pavel Shostak

To compile tests: make
To run tests: make test
To clean: make clean
To do it all: make clean && make && make test

All tests should be passing!

Notes:
Shouldn't both Set_Size and Set_Elem return an int so the user
knows if they executed correctly via a return vode?

For Myself:
No idea how to test if memory management is done correctly using
chase.
*/

#include <iostream>
#include "Square_Matrix.h"
#include "Sodoku.h"

// For the testing suite.
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
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
	GIVEN("an empty matrix"){
		Square_Matrix my_matrix;

		WHEN("checking it's contents"){
			THEN("it should not have any data")
				REQUIRE(my_matrix.Get_Elem(0,0) == 0);

			THEN("it should not have any data using Get_Row"){
				vector<int> temp = my_matrix.Get_Row(0);
				bool empty = true;

				for (int i = 0; i < my_matrix.Get_Size(); ++i)
					if(temp[i] != 0){
						bool empty = false;
						break;
					}

				REQUIRE(empty);
			}

			THEN("it should not have any data using Get_Column"){
				vector<int> temp = my_matrix.Get_Column(0);
				bool empty = true;

				for (int i = 0; i < my_matrix.Get_Size(); ++i)
					if(temp[i] != 0){
						bool empty = false;
						break;
					}

				REQUIRE(empty);
			}


			THEN("it should have 0 size")
				REQUIRE(my_matrix.Get_Size() == 0);
		}

		WHEN("adding in data"){
		my_matrix.Set_Elem(100, 0, 0);

			THEN("nothing should change")
				REQUIRE(my_matrix.Get_Elem(0,0) == 0);
		}
	}

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

		WHEN("Changing out of bounds data"){
		my_matrix.Set_Elem(100, 5, 5);

			THEN("should return zero'd data")
				REQUIRE(my_matrix.Get_Elem(5,5) == 0);
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

			THEN("out of bound data changes take no effect")
			{
				my_matrix.Set_Elem(101, 1, 1);
				REQUIRE(my_matrix.Get_Elem(5,5) == 0);
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

		WHEN("An element is requested") {
			int my_element = m.Get_Elem(5, 5);

			THEN("The element is returned") {
				REQUIRE(my_element == 0);
			}
		}

		WHEN("An out of range element is requested") {
			int my_element = m.Get_Elem(15, 15);

			THEN("The out of range response is recieved") {
				REQUIRE(my_element == 0);
			}
		}

		WHEN("An element is set") {
			// Set cell 5,6 to the value 3
			m.Set_Elem(3, 5, 6);

			THEN("The element should not be changed") {
				REQUIRE(m.Get_Elem(5, 6) == 0);
			}
		}

		WHEN("An out of range element is set") {
			// Set cell 5,6 to the value 3
			m.Set_Elem(3, 15, 15);

			THEN("Things should not blow up") {
				// I guess a segfault should have happened by now which
				// would have shown the tester things went wrong anyways.
				// If allowed to return an int, then can check return code.
				REQUIRE(m.sanity_check() == 2014);
			}
		}

		WHEN("the size is reset with old data") {
			// Set cell 6,7 to the value 2
			m.Set_Elem(2, 6, 7);

			m.Set_Size(12);

			THEN("the data should be lost") {
				REQUIRE(m.Get_Elem(6, 7) == 0);
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
			sodoku puzzle;

			THEN("The Sodoku puzzle should be 2 by 2 units big") {
				REQUIRE(puzzle.Get_Size() == 2);
			}
		}
	}
}

SCENARIO("Modfying and accessing cells of the Sodoku puzzle") {
	GIVEN("A empty non partially filled sodoku puzzle") {
		sodoku puzzle;

		WHEN("acessing an cell") {
			int cell_contents = puzzle.Get_Cell(0,1);

			THEN("cell contents should be unset(-1)") {
				REQUIRE(cell_contents == -1);
			}
		}

		WHEN("writing to an out of bounds cell") {
			bool successful_write = puzzle.Set_Cell(98, 99, 5);

			THEN("the return value should be false") {
				REQUIRE(!successful_write);
			}
		}

		WHEN("writing to an in bounds cell") {
			bool successful_write = puzzle.Set_Cell(0, 1, 5);

			THEN("The cell contents should be changed") {
				REQUIRE(successful_write);
				REQUIRE(puzzle.Get_Cell(0,1) == 5);
			}
		}
	}
}

SCENARIO("Checking an incorrect sodoku") {
	GIVEN("A sodoku puzzle with a bad row in it") {
		sodoku puzzle;
		puzzle.Set_Cell(0, 0, 5);
		puzzle.Set_Cell(1, 0, 5);

		WHEN("checking if there is a bad row") {
			bool validity = puzzle.check_row_validity(0);

			THEN("the return value should be false") {
				REQUIRE(!validity);
			}
		}

		WHEN("checking the entire sodoku puzzle") {
			bool validity = puzzle.check_sodoku_validity();

			THEN("the return value should be false") {
				REQUIRE(!validity);
			}
		}
	}

	GIVEN("A sodoku puzzle without a bad row in it") {
		sodoku puzzle;
		puzzle.Set_Cell(0, 0, 5);
		puzzle.Set_Cell(1, 1, 5);

		WHEN("checking if there is a bad row") {
			bool validity = puzzle.check_row_validity(0);

			THEN("the return value should be true") {
				REQUIRE(validity);
			}
		}

		WHEN("checking the entire sodoku puzzle") {
			bool validity = puzzle.check_sodoku_validity();

			THEN("the return value should be true") {
				REQUIRE(validity);
			}
		}
	}

	GIVEN("A sodoku puzzle with a bad column in it") {
		sodoku puzzle;
		puzzle.Set_Cell(0, 0, 5);
		puzzle.Set_Cell(0, 1, 5);

		WHEN("checking if there is a bad column") {
			bool validity = puzzle.check_column_validity(0);

			THEN("the return value should be false") {
				REQUIRE(!validity);
			}
		}

		WHEN("checking the entire sodoku puzzle") {
			bool validity = puzzle.check_sodoku_validity();

			THEN("the return value should be false") {
				REQUIRE(!validity);
			}
		}
	}

	GIVEN("A sodoku puzzle without a bad column in it") {
		sodoku puzzle;
		puzzle.Set_Cell(0, 0, 5);
		puzzle.Set_Cell(1, 1, 5);

		WHEN("checking if there is a bad column") {
			bool validity = puzzle.check_column_validity(0);

			THEN("the return value should be true") {
				REQUIRE(validity);
			}
		}

		WHEN("checking the entire sodoku puzzle") {
			bool validity = puzzle.check_sodoku_validity();

			THEN("the return value should be true") {
				REQUIRE(validity);
			}
		}
	}
}

SCENARIO("Filling in a few starting cells.") {
	GIVEN("An empty non partially filled sodoku puzzle") {
		sodoku puzzle;

		WHEN("filling in a few cells to get started") {
			puzzle.partial_fill(0.15);

			THEN("a few cells should be correctly filled") {
				puzzle.display();
				REQUIRE(puzzle.check_sodoku_validity());
			}
		}
	}
}

SCENARIO("Checking completness of the puzzle") {
	GIVEN("An empty sodoku puzzle") {
		sodoku puzzle;

		WHEN("checking if it is complete") {
			bool completness = puzzle.is_complete();

			THEN("it should be not complete") {
				REQUIRE(!completness);
			}
		}
	}

	GIVEN("A partially filled sodoku puzzle") {
		sodoku puzzle;

		// 1 0
		// 0 .
		puzzle.Set_Cell(0, 0, 1);
		puzzle.Set_Cell(0, 1, 0);
		puzzle.Set_Cell(1, 0, 0);

		WHEN("checking if it is complete") {
			bool completness = puzzle.is_complete();

			THEN("it should be not complete") {
				REQUIRE(!completness);
			}
		}
	}

	GIVEN("An incorrectly filled sodoku puzzle") {
		sodoku puzzle;

		// 1 0
		// 0 0
		puzzle.Set_Cell(0, 0, 1);
		puzzle.Set_Cell(0, 1, 0);
		puzzle.Set_Cell(1, 0, 0);
		puzzle.Set_Cell(1, 1, 0);

		WHEN("checking if it is complete") {
			bool completness = puzzle.is_complete();

			THEN("it should be not complete") {
				REQUIRE(!completness);
			}
		}
	}

	GIVEN("An correctly filled sodoku puzzle") {
		sodoku puzzle;

		// 1 0
		// 0 1
		puzzle.Set_Cell(0, 0, 1);
		puzzle.Set_Cell(0, 1, 0);
		puzzle.Set_Cell(1, 0, 0);
		puzzle.Set_Cell(1, 1, 1);

		WHEN("checking if it is complete") {
			bool completness = puzzle.is_complete();

			THEN("it should be not complete") {
				REQUIRE(completness);
			}
		}
	}
}
