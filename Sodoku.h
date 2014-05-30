#ifndef Sodoku_H
#define Sodoku_H

#include "Square_Matrix.h"
#include <string>

class Sodoku
{
	public:
		// Constructor for the Soduku object with a default size of 2.
		Sodoku(void);

		// Overloaded constructor for the Soduku object with a configurable
		// size is given, the size will be set to 0.
		Sodoku(int size);

		// DeConstructor for the Soduku object.
		~Sodoku(void);

		// Wipe the contents of the puzzle. This is used when resetting
		// the puzzle. Everything but the size of the puzzle is reset.
		void wipe(void);

		// Throws out the current soduku puzzle's contents with a message.
		void display(std::string input_string = "");

		// Displays the size of the Sodoku puzzle
		int get_size(void);

		// Gets a cell from the puzzle using x and y coordinates.
		// If out of bounds, returns a 0.
		int get_cell(const int& x_coordinate, const int& y_coordinate);

		// Sets a cell at x and y coordinates to some value. If out of bounds, returns false.
		bool set_cell(const int& x_coordinate, 
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
		void solve_puzzle_partially(const float&);

		// Fill specified amount of cells.
		void solve_puzzle_partially_count(const int& trying_to_fill);

		// Checks if the soduko puzzle is complete, meaning if it is valid and every
		// cell is filled. This does not ignore cells which are unset.
		bool is_complete(void);

		// Solves the puzzle based on solve_status.
		bool solve_puzzle(void);

		// THIS IS FOR UNIT TESTING ONLY! DON'T USE ME!!
		// Not in private because then it couldn't be accessed by the unit tests.
		void set_const_cell(int x, int y, int value);

		// Returns an idea of how "hard" it was to find the solution.
		int get_amount_of_steps(void);

		void display_heatmap(std::string input_string = "");
		void display_writable(std::string input_string = "");

	private:
		Square_Matrix matrix;

		// Counts how many cells have been filled already.
		int count_filled_cells(void);

		// Coordinates of a cell which is not allowed to change. Meant for holding
		// cells which were partially filled when the puzzle is first presented.
		struct coordinates
		{
			int x;
			int y;
		};

		// The coordinates of the current cell we are trying to fill.
		coordinates working_cell;

		// Used to increment and decrement the coordinates of working_cell().
		// Returns false if we can't get to the next/previous valid cell.
		bool next_cell(void);
		bool back_cell(void);

		// Increments the cell by one. This is quicker than having to get the cell
		// and then set the cell using two LOC, or three if you want it to look nice.
		// Returns a zero if the new value will be out of bounds or if you can't write
		// to that cell because it is a constant.
		bool increment_cell_contents(const int& column, const int& row);

		// Checks if the input coordinates exist in the vector of coordinates 
		// for cells which we are not allowed to modify.
		bool can_set(int x, int y);

		// Try to fill the cell at x and y, if fail then return a false.
		bool try_to_fill(const int &x, const int &y);

		// Keeps track of how many attempts or tries there were for cells.
		// Each write counts as an attempt or try. While not ideal for
		// measuring how difficult it was, it gives a good idea.
		int amount_of_steps;

		// Heatmap for attempts per cell.
		Square_Matrix heatmap;

		// Map saying if we can or can't write to the cell (partial_fill'ed).
		Square_Matrix writable;
};

#endif
