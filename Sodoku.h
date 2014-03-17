#ifndef Sodoku_H
#define Sodoku_H

#include "Square_Matrix.h"
#include <string>

class sodoku
{
	public:
		// Constructor for the Soduku object with a default size of 2.
		sodoku(void);

		// Overloaded constructor for the Soduku object with a configurable
		// size is given, the size will be set to 0.
		sodoku(int size);

		// DeConstructor for the Soduku object.
		~sodoku(void);

		// Throws out the current soduku puzzle's contents with a message.
		void display(std::string input_string = "");

		// Displays the size of the Sodoku puzzle
		int Get_Size(void);

		// Gets a cell from the puzzle using x and y coordinates.
		// If out of bounds, returns a 0.
		int Get_Cell(const int& x_coordinate, const int& y_coordinate);

		// Sets a cell at x and y coordinates to some value. If out of bounds, returns false.
		bool Set_Cell(const int& x_coordinate, 
					  const int& y_coordinate, 
					  const int& new_value);

		// Checks if the row is valid, meaning if there are multiples if a number.
		bool check_row_validity(const int& row);

		// Checks if the column is valid, meaning if there are multiples if a number.
		bool check_column_validity(const int& column);

		// Checks if the entire Sodoku puzzle is correct, combines checking each row
		// and column into just one function call. This ignores cells which are unset.
		bool check_sodoku_validity(void);

		// Fill in a a percentage of cells with at least one filled.
		void partial_fill(const float&);

		// FOR UNIT TESTING ONLY! DON'T USE ME
		void set_const_cell(int value, int x, int y);

		// Checks if the soduko puzzle is complete, meaning if it is valid and every
		// cell is filled. This does not ignore cells which are unset.
		bool is_complete(void);

		// Increments the cell by one. This is quicker than having to get the cell
		// and then set the cell using two LOC, or three if you want it to look nice.
		// Returns a zero if the new value will be out of bounds or if you can't write
		// to that cell because it is a constant.
		bool increment_cell(const int& column, const int& row);

		// Solves the puzzle based on solve_status.
		bool solve_puzzle(void);
		
		// Decrements the cell by one. Same as increment but decreases by one.
		bool decrement_cell(const int& column, const int& row);

	private:
		Square_Matrix matrix;

		// Counts how many cells have been filled already.
		int count_filled_cells(void);

		// Used solely to keep track of how many cells we want to fill when using
		// partial_fill().
		int trying_to_fill;

		// Coordinates of a cell which is not allowed to change. Meant for holding
		// cells which were partially filled when the puzzle is first presented.
		struct coordinates
		{
			int x;
			int y;
		};

		// Holds a vector of cell coordinates we are not allowed to modify later.
		std::vector<coordinates> const_cells;

		// The coordinates of the current cell we are trying to fill.
		coordinates working_cell;

		// Used to increment and decrement the coordinates of working_cell().
		// Returns false if we can't get to the next/previous valid cell.
		bool next_cell(void);
		bool back_cell(void);

		// Checks if the input coordinates exist in the vector of coordinates 
		// for cells which we are not allowed to modify.
		bool can_set(int x, int y);

		// Try to fill the cell at x and y, if fail then return a false.
		bool try_to_fill(const int &x, const int &y);

		// Keeps track of how many steps we took to solve the puzzle.
		int amount_of_steps;

		// Flag to view how the puzzle gets solved step by step.
		bool view_progress;
};

#endif
