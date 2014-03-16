#include "Sodoku.h"
#include "Square_Matrix.h"
#include <time.h> // Solely for seeding the RNG.
#include <stdlib.h> // Solely for the RNG.
#include <iostream>

// NOTE:
// Because I do not wish to modify Square_Matrix from the previous assignment, 
// some issues are dealt with in this project. For example, to deal with cells
// existing but not yet written to, I fill the entire matrix with -1's and if a 
// -1 is found it is considered to be not yet written to. I don't know how to
// handle Get_Cell.

// Gotta use a namespace or else all my couts will have to specify the namespace
// each time.
using namespace std;

// Constructor for the sodoku object.
sodoku::sodoku(void){
	this->matrix.Set_Size(2);
	this->matrix.fill(-1);
	this->solve_status = false;
	this->trying_to_fill = 0;

	// Seed the random number generator using the time.
	srand (time(NULL));
}

// DeConstructor for the sodoku object.
sodoku::~sodoku(void){

}

int sodoku::Get_Size(void){
	return this->matrix.Get_Size();
}

// Throws out the current sodoku puzzle contents by going row through row, and
// adding spacing between each cells contents for making it easier to read.
void sodoku::display(void){
	// For saying that these are the contents.
	cout << "Contents of puzzle: " << endl;

	// Will hold the current row.
	vector<int> row_contents;

	// Go through each row.
	for (int row = 0; row < this->matrix.Get_Size(); ++row)
	{
		// Copies the row into a vector.
		row_contents = this->matrix.Get_Row(row);

		// Makes some nice formatting to the start of the row.
		cout << "| ";

		// Displays the contents of each cell of the row with right hand padding.
		for (int cell_count = 0; cell_count < this->matrix.Get_Size(); ++cell_count)
		{
			if (row_contents[cell_count] == -1)
				cout << ". ";
			else
				cout << row_contents[cell_count] << " ";
		}

		// Shows that the row is done.
		cout << "|" << endl;
	}
}

// Returns the contents of the cell at the X and Y coordinates. If out of bounds,
// returns a 0, of which the behavior is implemented in the Square_Matrix lib.
int sodoku::Get_Cell(const int &x, const int &y){
	return this->matrix.Get_Elem(x, y);
}

// Sets a cell at x and y coordinates to some value. If out of bounds, returns false.
bool sodoku::Set_Cell(const int& x, const int& y, const int& val){

	// If the coordinates are out of bounds, return false.
	if (x > this->matrix.Get_Size() - 1 || y > this->matrix.Get_Size() - 1)
		return false;

	// Sets the element. Val is first because previous assignment required it to be so.
	this->matrix.Set_Elem(val, x, y);

	// If we got here, then it was in bounds, so return a true.
	return true;
}

// Checks if the row is valid, meaning if there are multiples if a number.
// This goes through each element in the row and checks if that element exists 
// elsewhere in the row.
bool sodoku::check_row_validity(const int& row){
	// Holds the row of int's
	std::vector<int> row_contents = this->matrix.Get_Row(row);

	// Go through each row.
	for (int i = 0; i < row_contents.size(); ++i)
	{
		// Check if more than 1 of that int exists in the row.
		for (int j = 0; j < row_contents.size(); ++j)
		{
			// If the number appears twice and it is not an unset cell and it is
			// not checking itself, then that row is invalid.
			if ((row_contents[i] == row_contents[j]) && (row_contents[i] != -1) && (i != j))
				return false;
		}
	}

	// If we get here, it means that the row is valid.
	return true;
}

// Checks if column is valid, meaning if there are multiples if a number.
// This goes through each element in the column and checks if that element exists 
// elsewhere in the column.
bool sodoku::check_column_validity(const int& column){
	// Holds the column of int's
	std::vector<int> column_contents = this->matrix.Get_Column(column);

	// Go through each column.
	for (int i = 0; i < column_contents.size(); ++i)
	{
		// Check if more than 1 of that int exists in the column.
		for (int j = 0; j < column_contents.size(); ++j)
		{
			// If the number appears twice and it is not an unset cell and it is
			// not checking itself, then that column is invalid.
			if ((column_contents[i] == column_contents[j]) && (column_contents[i] != -1) && (i != j))
				return false;
		}
	}

	// If we get here, it means that the column is valid.
	return true;
}

// Checks if the entire Sodoku puzzle is correct, combines checking each row
// and column into just one function call.
bool sodoku::check_sodoku_validity(void){

	// Since this is a square, we can check rows and columns in one for loop.
	for (int i = 0; i < this->matrix.Get_Size(); ++i)
	{
		if ( this->check_row_validity(i) != true)
			return false;

		if ( this->check_column_validity(i) != true)
			return false;
	}

	// If we get here, it means that the puzzle is valid.
	return true;
}

// Fill in a a percentage of cells with at least one filled. Also adds the cells
// coordinates into a vector which is used to differentiate between nodes which
// can and can't be changed when attempting to solve the puzzle.
//
// 		NOTE
// This ONLY works for small percentages since it can only go back one "node",
// so if it turns out that no matter what you try to the current node and it 
// still won't be valid, then this will fail.
void sodoku::partial_fill(const float& percentage){
	// First we need to find out how many cells to fill.
	int total_cell_count = this->matrix.Get_Size() * this->matrix.Get_Size();
	this->trying_to_fill = total_cell_count * percentage;

	// We need to make sure there is at least one partially filled cell for
	// really small puzzles.
	if (this->trying_to_fill < 1) {this->trying_to_fill = 1;}
	
	// Keeps track of how many cells were filled.
	int filled = 0;

	// While the filled cells is less than the amount of cells we are trying to
	// fill, keep this loop going.
	while (filled != this->trying_to_fill)
	{
		// Make a random set of coordinates and a random int.
		int var = rand() % ( this->matrix.Get_Size() );
		int x_coordinate = rand() % ( this->matrix.Get_Size() );
		int y_coordinate = rand() % ( this->matrix.Get_Size() );

		// If this coordinate has not been already set earlier, try to put a
		// new value into it.
		if (this->matrix.Get_Elem(x_coordinate, y_coordinate) == -1)
		{
			// Write the cell.
			this->matrix.Set_Elem(var, x_coordinate, y_coordinate);

			// Check if the changes are valid, if not then reverse the changes,
			// if they are good then add them into the const_cells entry.
			if (this->check_sodoku_validity())
			{
				// Add the new coordinates to the list.
				const_cells.push_back( coordinates() );
				const_cells[filled].x = x_coordinate;
				const_cells[filled].y = y_coordinate;

				// Increment the amount of filled cells.
				filled++;
			}
			// If the new cell is not valid, reset that cell back to its original
			// state.
			else
				this->matrix.Set_Elem(-1, x_coordinate, y_coordinate);
		}
	}
}

// Solves the puzzle based on solve_status.
sodoku solve_puzzle(sodoku& previous_sodoku){
}

// Increments the cell by one. This is quicker than having to get the cell
// and then set the cell using two LOC, or three if you want it to look nice.
// Returns a zero if the new value will be out of bounds or if you can't write
// to that cell because it is a constant.
bool sodoku::increment_cell(const int& row, const int& column){
	if ( row < this->matrix.Get_Size() && // Is the row out of bounds?
		 column < this->matrix.Get_Size() && // Is the column out of bounds?
		 this->can_set(row, column) && // Is the cell writable?
		 this->Get_Cell(row, column) < this->matrix.Get_Size() ) // Is the new
		// value out of bounds?
	{
		// Holds the contents of the cell.
		int cell_contents = this->Get_Cell(row, column);

		// Increment the contents by one.
		cell_contents++;

		// Save the new contents into the cell.
		this->matrix.Set_Elem(cell_contents, row, column);

		// We were able to save the new cell contents, so all went well!
		return true;
	}

	// If we got here, then something went wrong, so return a false.
	else {return false;}
}
		return true;
	}
	else {return false;}
}

// Checks if the sodoku puzzle is complete, meaning if it is valid and every
// cell is filled.
bool sodoku::is_complete(void){
	// First we see if every cell is filled.
	if ( this->count_filled_cells() != (this->matrix.Get_Size() * this->matrix.Get_Size()) )
		return false;

	// Secondly, if the contents of the puzzle are valid.
	if ( !this->check_sodoku_validity() )
		return false;

	// If we get here, it means the sodoku puzzle is complete.
	return true;
}

// Counts how many cells have been filled already.
int sodoku::count_filled_cells(void){
	// Holds the counter for how many cells have been set.
	int filled_cell_count = 0;

	// Go through each column.
	for (int i = 0; i < this->matrix.Get_Size(); ++i)
	{
		// Check if more than 1 of that int exists in the column.
		for (int j = 0; j < this->matrix.Get_Size(); ++j)
		{
			// If the number appears twice and it is not an unset cell and it is
			// not checking itself, then that column is invalid.
			if (this->matrix.Get_Elem(i, j) != -1)
				filled_cell_count++;
		}
	}

	return filled_cell_count;
}

// Checks if the sodoku puzzle is complete, meaning if it is valid and every
// cell is filled.
bool sodoku::can_set(int x_coordinate, int y_coordinate){
	for (int i = 0; i < const_cells.size(); ++i)
		if ((const_cells[i].x == x_coordinate) && (const_cells[i].y == y_coordinate));
			return false;

	return true;
}
