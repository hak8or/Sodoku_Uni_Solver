#ifndef Sodoku_H
#define Sodoku_H

#include "Square_Matrix.h"

class sodoku
{
	public:
		// Constructor for the Soduku object.
		sodoku(void);

		// DeConstructor for the Soduku object.
		~sodoku(void);

		// Throws out the current soduku puzzle's contents.
		void display(void);

		// Displays the size of the Sodoku puzzle
		int Get_Size(void);

		// Gets a cell from the puzzle using x and y coordinates.
		// If out of bounds, returns a 0.
		int Get_Cell(const int&, const int&);

		// Sets a cell at x and y coordinates to some value. If out of bounds, returns false.
		bool Set_Cell(const int& x_coordinate, 
					  const int& y_coordinate, 
					  const int& new_value);

		// Checks if the row is valid, meaning if there are multiples if a number.
		bool check_row_validity(const int& row);

		// Checks if the column is valid, meaning if there are multiples if a number.
		bool check_column_validity(const int& column);

		// Checks if the entire Sodoku puzzle is correct, combines checking each row
		// and column into just one function call.
		bool check_sodoku_validity(void);

		// Fill in a a percentage of cells with at least one filled.
		void partial_fill(float);

		// Checks if the soduko puzzle is complete, meaning if it is valid and every
		// cell is filled.
		bool is_complete(void);

	private:
		Square_Matrix matrix;

		// Counts how many cells have been filled already.
		int count_filled_cells(void);

		// Bool used to say if currently trying to fill it or just some cells.
		// 1 = solve everything
		// 0 = solve just some
		bool solve_status;

		// Used solely to keep track of how many cells we want to fill when using
		// partial_fill().
		int trying_to_fill;

		// Solves the puzzle based on solve_status.
		sodoku solve_puzzle(sodoku&);

		// Coordinates of a cell which is not allowed to change. Meant for holding
		// cells which were partially filled when the puzzle is first presented.
		struct coordinates
		{
			int x;
			int y;
		};

		// Holds a vector of cell coordinates we are not allowed to modify later.
		std::vector<coordinates> const_cells;

		// Latest cell tried to fill
		coordinates attempted_cell;

		// Checks if the input coordinates exist in the vector of coordinates for cells
		// we are not allowed to modify.
		bool can_set(int x, int y);

};

#endif
